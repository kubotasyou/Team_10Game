#include "Sprite.h"

#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//静的メンバ変数
ID3D12Device* Sprite::device = nullptr;
UINT Sprite::descriptorHandleIncrementSize;
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Sprite::rootSignature;
ComPtr<ID3D12PipelineState> Sprite::pipelineState;
XMMATRIX Sprite::matProjection;
ComPtr<ID3D12DescriptorHeap> Sprite::descHeap;
ComPtr<ID3D12Resource> Sprite::texBuff[srvCount];

void Sprite::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	//引数の受け取り
	Sprite::device = device;

	//ディスクリプタサイズを取得
	descriptorHandleIncrementSize =
		device->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);

	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob> vsBlob;//頂点シェーダーオブジェクト
	ComPtr<ID3DBlob> psBlob;//ピクセルシェーダーオブジェクト
	ComPtr<ID3DBlob> errorBlob;//エラーオブジェクト

#pragma region シェーダーの読み込みとコンパイル

	//頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"SpriteVertexShader.hlsl",                     //シェーダー名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,              //インクルードを可能にする
		"VSmain",                                       //エントリーポイントの指定
		"vs_5_0",                                       //シェーダーモデルの指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
		0,
		&vsBlob, &errorBlob
	);
	//読み取りエラー時の処理
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"SpritePixelShader.hlsl",                     //シェーダー名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,              //インクルードを可能にする
		"PSmain",                                       //エントリーポイントの指定
		"ps_5_0",                                       //シェーダーモデルの指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
		0,
		&psBlob, &errorBlob
	);
	//読み取りエラー時の処理
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion

#pragma region 頂点レイアウト設定

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = 
	{
		{//xyz座標
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0,
        },
	    {//uv座標
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0,
        }
	};

#pragma endregion

#pragma region グラフィクスパイプラインの作成

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//シェーダーのセット---------------------------------
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	//---------------------------------------------------

	//サンプルマスク(標準設定)
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//ラスタライズステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//背面カリングをしない
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//常に上書きできる設定にする
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	//ブレンドの設定--------------------------------------------------
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	//RGBA全てのチャンネルを描画(標準設定)
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//共通設定
	blendDesc.BlendEnable = true;//ブレンドを有効にする
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;//加算合成
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのα値
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストのα値

	//加算合成の設定
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;  //ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

	//ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blendDesc;
	//----------------------------------------------------------------

	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定------------------------------------
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	//--------------------------------------------------------

	//図形の形状の設定
	gpipeline.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//描画対象の数
	gpipeline.NumRenderTargets = 1;

	//0～1に正規化されたRGBA
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	//1ピクセルにつき1回のサンプリング
	gpipeline.SampleDesc.Count = 1;

	//ディスクリプタレンジ------------------------------------
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	//t0レジスタ
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	//--------------------------------------------------------

	//ルートパラメーター----------------------------------
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0,
		D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV,
		D3D12_SHADER_VISIBILITY_ALL);
	//----------------------------------------------------

	//スタティックサンプラー(s0レジスタ)
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc =
		CD3DX12_STATIC_SAMPLER_DESC(0,
			D3D12_FILTER_MIN_MAG_MIP_POINT);

	//ルートシグネチャの設定------------------------------------------
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams),
		rootparams, 1, &samplerDesc, 
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//----------------------------------------------------------------

	ComPtr<ID3DBlob> rootSigBlob;
	//バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob
	);

	//ルートシグネチャの生成
	result = device->CreateRootSignature(0,
		rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	);

	//ルートシグネチャの設定を反映
	gpipeline.pRootSignature = rootSignature.Get();

	//グラフィクスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(&pipelineState)
	);

#pragma endregion

	//ディスクリプタヒープを生成-----------------------------------
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//シェーダーから見えるようにする。
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = srvCount;
	result = device->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(&descHeap)
	);
	//-------------------------------------------------------------

	//射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f,
		(float)window_width,
		(float)window_height,
		0.0f, 0.0f, 1.0f
	);

}

void Sprite::LoadTexture(UINT texNumber, const wchar_t * fileName)
{
	if (device == nullptr)
	{
		assert("デバイス内");
	}

	HRESULT result = S_FALSE;

#pragma region テクスチャの読み込み

	//画像のサイズとテクスチャとしての情報が入る。
	TexMetadata metadata{};
	//画像のデータのアドレスが入る。
	ScratchImage scratchImg{};

	//テクスチャの読み込み
	result = LoadFromWICFile(
		fileName,
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	//生データの抽出
	const Image* img = scratchImg.GetImage(0, 0, 0);

	//リソースの設定
	//読み込んだデータををテクスチャデータに設定する
	CD3DX12_RESOURCE_DESC texresDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels
		);

	//テクスチャバッファの作成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texBuff[texNumber])
	);

	//テクスチャバッファにデータを転送
	result = texBuff[texNumber]->WriteToSubresource(
		0,
		nullptr,             //全領域にコピー
		img->pixels,         //元のデータアドレス
		(UINT)img->rowPitch, //1ラインサイズ
		(UINT)img->slicePitch//1枚サイズ
	);

#pragma endregion

#pragma region シェーダーリソースビュー作成

	//設定構造体
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//2Dテクスチャ
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//生成
	device->CreateShaderResourceView(texBuff[texNumber].Get(),
		&srvDesc,//テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(),
			texNumber,descriptorHandleIncrementSize)
	);

#pragma endregion

}

void Sprite::BeginDraw(ID3D12GraphicsCommandList * cmdList)
{
	//コマンドリストをセットする
	Sprite::cmdList = cmdList;

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::EndDraw()
{
	//コマンドリスト解除
	Sprite::cmdList = nullptr;
}

Sprite * Sprite::Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 anchorPoint, bool isFlipX, bool isFlipY)
{
	//初期サイズ
	XMFLOAT2 size = { 100.0f,100.0f };

	if (texBuff[texNumber])
	{
		//テクスチャの情報取得
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();
		//スプライトのサイズをテクスチャのサイズに設定
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	//Spriteのインスタンスを生成
	Sprite* sprite = new Sprite(texNumber, position, size, 
		color, anchorPoint, isFlipX, isFlipY);

	//初期化
	sprite->Initialize();

	return sprite;
}

Sprite::Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorPoint, bool isFlipX, bool isFlipY)
{
	this->position = position;
	this->size = size;
	this->anchorPoint = anchorPoint;
	this->matWorld = XMMatrixIdentity();
	this->color = color;
	this->texNumber = texNumber;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;
	this->texSize = size;
}

void Sprite::Initialize()
{
	HRESULT result = S_FALSE;

#pragma region 頂点情報

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexData)*vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	//頂点バッファへデータ転送
	SendVertexData();

	//頂点バッファービューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//合計バイト数を求める
	vbView.SizeInBytes = sizeof(VertexData) * 4;
	//1頂点当たりのバイト数を調べる.
	vbView.StrideInBytes = sizeof(VertexData);

#pragma endregion

#pragma region 定数情報

	//定数バッファの作成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能に
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	//定数バッファにデータを転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}

#pragma endregion

}

void Sprite::SetRotation(float rotation)
{
	//値を引数で変更して、頂点バッファーへ転送する

	this->rotationZ = rotation;

	//頂点バッファへのデータ転送
	SendVertexData();
}

void Sprite::SetPosition(XMFLOAT2 position)
{
	//値を引数で変更して、頂点バッファーへ転送する

	this->position = position;

	//頂点バッファへのデータ転送
	SendVertexData();
}

void Sprite::SetScale(XMFLOAT2 size)
{
	//値を引数で変更して、頂点バッファーへ転送する

	this->size = size;

	//頂点バッファへのデータ転送
	SendVertexData();
}

void Sprite::SetColor(XMFLOAT4 color)
{
	this->color = color;

	SendVertexData();
}

void Sprite::SetAnchorPoint(XMFLOAT2 anchorPoint)
{
	//値を引数で変更して、頂点バッファーへ転送する

	this->anchorPoint = anchorPoint;

	//頂点バッファへのデータ転送
	SendVertexData();
}

void Sprite::SetFlitX(bool isFlipX)
{
	//値を引数で変更して、頂点バッファーへ転送する

	this->isFlipX = isFlipX;

	//頂点バッファへのデータ転送
	SendVertexData();
}

void Sprite::SetFlitY(bool isFlipY)
{
	//値を引数で変更して、頂点バッファーへ転送する

	this->isFlipY = isFlipY;

	//頂点バッファへのデータ転送
	SendVertexData();
}

void Sprite::SetTextureRect(XMFLOAT2 texBasePos, XMFLOAT2 texSize)
{
	this->texBasePos = texBasePos;
	this->texSize = texSize;

	//頂点バッファへのデータ転送
	SendVertexData();
}

void Sprite::Draw()
{
	//ワールド行列の更新------------------------------------------------
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);
	//------------------------------------------------------------------

	//定数バッファにデータを転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection;
		this->constBuff->Unmap(0, nullptr);
	}

	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	//ディスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//定数バッファービューをセット
	cmdList->SetGraphicsRootConstantBufferView(0,
		this->constBuff->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			this->texNumber,
			descriptorHandleIncrementSize)
	);

	//描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SendVertexData()
{
	HRESULT result = S_FALSE;

	enum {
		LB,// 0 -- 左下
		LT,// 1 -- 左上
		RB,// 2 -- 右下
		RT // 3 -- 右上
	};

	//座標の初期値
	float left =   (0.0f - anchorPoint.x) * size.x;
	float right =  (1.0f - anchorPoint.x) * size.x;
	float top =    (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	//左右入れ替え
	if (isFlipX)
	{
		left = -left;
		right = -right;
	}
	//上下入れ替え
	if (isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}


	//頂点データ
	VertexData vertices[vertNum];

	//座標初期化
	vertices[LB].pos = { left,bottom, 0.0f };
	vertices[LT].pos = { left,top,    0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top,   0.0f };

	//テクスチャ情報取得
	if (texBuff[texNumber])
	{
		D3D12_RESOURCE_DESC resDesc = 
			texBuff[texNumber]->GetDesc();

		//uv座標の初期値
		float tex_L = texBasePos.x / resDesc.Width;
		float tex_R = (texBasePos.x + texSize.x) / resDesc.Width;
		float tex_T = texBasePos.y / resDesc.Height;
		float tex_B = (texBasePos.y + texSize.y) / resDesc.Height;

		//uv座標初期化
		vertices[LB].uv = { tex_L,tex_B };
		vertices[LT].uv = { tex_L,tex_T };
		vertices[RB].uv = { tex_R,tex_B };
		vertices[RT].uv = { tex_R,tex_T };
	}

	//頂点バッファーへデータ転送
	VertexData* vertMap = nullptr;

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

