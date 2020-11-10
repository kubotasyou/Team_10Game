#include "Object3D.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma region 静的メンバ変数の実態

const float Object3D::radius = 5.0f;				// 底面の半径
const float Object3D::prizmHeight = 8.0f;			// 柱の高さ
ID3D12Device* Object3D::device = nullptr;
UINT Object3D::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* Object3D::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Object3D::rootsignature;
ComPtr<ID3D12PipelineState> Object3D::pipelinestate;
ComPtr<ID3D12DescriptorHeap> Object3D::descHeap;
ComPtr<ID3D12Resource> Object3D::vertBuff;
ComPtr<ID3D12Resource> Object3D::indexBuff;
ComPtr<ID3D12Resource> Object3D::texbuff;
CD3DX12_CPU_DESCRIPTOR_HANDLE Object3D::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE Object3D::gpuDescHandleSRV;
XMMATRIX Object3D::matView{};
XMMATRIX Object3D::matProjection{};
XMFLOAT3 Object3D::eye = { 0, 0, -50.0f };
XMFLOAT3 Object3D::target = { 0, 0, 0 };
XMFLOAT3 Object3D::up = { 0, 1, 0 };
D3D12_VERTEX_BUFFER_VIEW Object3D::vbView{};
D3D12_INDEX_BUFFER_VIEW Object3D::ibView{};
Object3D::Vertex Object3D::vertices[vertexCount];
unsigned short Object3D::indices[planeCount * 3];

#pragma endregion

//一度しか呼び出されない初期化
void Object3D::StaticInitialize(ID3D12Device * dev, int window_width, int window_height)
{
	//デバイスをセット
	Object3D::device = dev;

	//ディスクリプタヒープの初期化
	InitializeDescriptorHeap();

	//カメラ初期化
	InitializeCamera(window_width, window_height);

	//グラフィクスパイプライン初期化
	InitializeGraphicsPipeline();

	//テクスチャ読み込み
	Loadtexture();

	//モデル生成
	CreateModel();
}

//一度しか呼び出されない描画前処理
void Object3D::BeginDraw(ID3D12GraphicsCommandList * cmdList)
{
	// コマンドリストをセット
	Object3D::cmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//一度しか呼び出されない描画後処理
void Object3D::EndDraw()
{
	//コマンドリストを解除
	Object3D::cmdList = nullptr;
}

Object3D * Object3D::Create()
{
	//3Dオブジェクトインスタンスを生成
	Object3D* object3d = new Object3D();

	//初期化
	object3d->Initialize();

	return object3d;
}

void Object3D::SetEye(XMFLOAT3 eyePos)
{
	Object3D::eye = eyePos;

	UpdateViewmatrix();
}

void Object3D::SetTarget(XMFLOAT3 targetPos)
{
	Object3D::target = targetPos;

	UpdateViewmatrix();
}

void Object3D::CameraMoveVector(XMFLOAT3 velocity)
{
	XMFLOAT3 eye_move = GetEye();
	XMFLOAT3 target_mode = GetTarget();

	eye_move.x += velocity.x;
	eye_move.y += velocity.y;
	eye_move.z += velocity.z;

	target_mode.x += velocity.x;
	target_mode.y += velocity.y;
	target_mode.z += velocity.z;

	SetEye(eye_move);
	SetTarget(target_mode);

}

void Object3D::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	//ディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//シェーダーファイルから確認できるように
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//シェーダーリソースビューは一つ
	descHeapDesc.NumDescriptors = 1;

	//生成
	result = device->CreateDescriptorHeap(
		&descHeapDesc,
		IID_PPV_ARGS(&descHeap)
	);

	//ディスクリプタサイズを取得
	descriptorHandleIncrementSize =
		device->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);
}

void Object3D::InitializeCamera(int window_width, int window_height)
{
	//ビュー行列の生成
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);

	//透視投影による射影変換行列
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f,
		1000.0f
	);
}

void Object3D::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;

	//シェーダーオブジェクト生成
	ComPtr<ID3DBlob> vsBlob;   //頂点シェーダーオブジェクト
	ComPtr<ID3DBlob> psBlob;   //ピクセルシェーダーオブジェクト
	ComPtr<ID3DBlob> errorBlob;//エラーオブジェクト

#pragma region 各シェーダーの読み込みとコンパイル

	//頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVertexShader.hlsl",                     //シェーダー名
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
		L"BasicPixelShader.hlsl",                     //シェーダー名
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
			0
		},
		{//法線ベクトル
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0

		},
		{//uv座標
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
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
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//常に上書きできる設定にする
	//gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

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
		CD3DX12_STATIC_SAMPLER_DESC(0);

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
		IID_PPV_ARGS(&rootsignature)
	);

	//ルートシグネチャの設定を反映
	gpipeline.pRootSignature = rootsignature.Get();

	//グラフィクスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(&pipelinestate)
	);

#pragma endregion

}

void Object3D::Loadtexture()
{
	HRESULT result = S_FALSE;

#pragma region テクスチャの読み込み

	//画像のサイズとテクスチャとしての情報が入る。
	TexMetadata metadata{};
	//画像のデータのアドレスが入る。
	ScratchImage scratchImg{};

	//テクスチャの読み込み
	result = LoadFromWICFile(
		L"Resources/Texture/tex1.png",
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
			static_cast<UINT>(metadata.height),
			static_cast<UINT16>(metadata.arraySize),
			static_cast<UINT16>(metadata.mipLevels)
		);

	//テクスチャバッファの作成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);

	//テクスチャバッファにデータを転送
	result = texbuff->WriteToSubresource(
		0,
		nullptr,             //全領域にコピー
		img->pixels,         //元のデータアドレス
		static_cast<UINT>(img->rowPitch), //1ラインサイズ
		static_cast<UINT>(img->slicePitch)//1枚サイズ
	);

#pragma endregion

#pragma region シェーダーリソースビュー作成

	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		descHeap->GetCPUDescriptorHandleForHeapStart(),
		0,
		descriptorHandleIncrementSize
	);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descHeap->GetGPUDescriptorHandleForHeapStart(),
		0,
		descriptorHandleIncrementSize
	);

	//設定構造体
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//2Dテクスチャ
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//生成
	device->CreateShaderResourceView(texbuff.Get(),
		&srvDesc,//テクスチャ設定情報
		cpuDescHandleSRV
	);

#pragma endregion
}

void Object3D::CreateModel()
{
	HRESULT result = S_FALSE;

	std::vector<Vertex> realVertices;

#pragma region 頂点座標の計算(重複あり)
	{
		realVertices.resize((divison + 1) * 2);
		int index = 0;
		float zValue;

		//底面
		zValue = prizmHeight / 2.0f;
		for (int i = 0; i < divison; i++)
		{
			XMFLOAT3 vertex;
			vertex.x = radius * sinf(XM_2PI / divison * i);
			vertex.y = radius * cosf(XM_2PI / divison * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		//底面の中心点
		realVertices[index++].pos = XMFLOAT3(0, 0, zValue);

		//天面
		zValue = -prizmHeight / 2.0f;
		for (int i = 0; i < divison; i++)
		{
			XMFLOAT3 vertex;
			vertex.x = radius * sinf(XM_2PI / divison * i);
			vertex.y = radius * cosf(XM_2PI / divison * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		//天面の中心点
		realVertices[index++].pos = XMFLOAT3(0, 0, zValue);
	}

#pragma endregion

#pragma region 頂点の計算(重複無し)
	
	{
		int index = 0;
		//底面
		for (int i = 0; i < divison; i++)
		{
			unsigned short index0 = i + 1;
			unsigned short index1 = i;
			unsigned short index2 = divison;

			vertices[index++] = realVertices[index0];
			vertices[index++] = realVertices[index1];
			vertices[index++] = realVertices[index2];//底面の中心点
		}
		//底面の最後の三角形の一番目のインデックスを0に書き替え
		vertices[index - 3] = realVertices[0];

		int topStart = divison + 1;
		//天面
		for (int i = 0; i < divison; i++)
		{
			unsigned short index0 = topStart + i;
			unsigned short index1 = topStart + i + 1;
			unsigned short index2 = topStart + divison;

			vertices[index++] = realVertices[index0];
			vertices[index++] = realVertices[index1];
			vertices[index++] = realVertices[index2];//天面の中心点
		}
		//天面の最後の三角形の1番目のインデックスを0に書き替え
		vertices[index - 2] = realVertices[topStart];

		//側面
		for (int i = 0; i < divison; i++)
		{
			unsigned short index0 = i + 1;
			unsigned short index1 = topStart + i + 1;
			unsigned short index2 = i;
			unsigned short index3 = topStart + i;

			if (i == divison - 1)
			{
				index0 = 0;
				index1 = topStart;
			}

			vertices[index++] = realVertices[index0];
			vertices[index++] = realVertices[index1];
			vertices[index++] = realVertices[index2];

			vertices[index++] = realVertices[index2];
			vertices[index++] = realVertices[index1];
			vertices[index++] = realVertices[index3];
		}
	}

#pragma endregion

#pragma region 頂点インデックスの設定

	//頂点インデックスの設定
	for (int i = 0; i < _countof(indices); i++)
	{
		indices[i] = i;
	}

#pragma endregion

#pragma region 法線方向の計算

	for (int i = 0; i < _countof(indices) / 3; i++)
	{//三角形1つごとに計算していく
		//三角形のインデックスを取得
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0→p1ベクトル、p0→p2ベクトル計算
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//外積は両方から垂直なベクトルで外積
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//正規化
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データのnormalに代入
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}

#pragma endregion

	//頂点バッファ生成-----------------------------------
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	//---------------------------------------------------

	//インデックスバッファ生成---------------------------
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);
	//---------------------------------------------------

	//頂点バッファへデータ転送----------------------------
	Vertex* vertMap = nullptr;
	
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	memcpy(vertMap, vertices, sizeof(vertices));

	vertBuff->Unmap(0, nullptr);
	//----------------------------------------------------

	//インデクスバッファへデータ転送-----------------------
	unsigned short* indexMap = nullptr;

	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	//全インデックスについて
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}

	indexBuff->Unmap(0, nullptr);
	//-----------------------------------------------------

	//頂点バッファビューの作成-------------------------------
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
	//-------------------------------------------------------

	//インデクスバッファビューの作成--------------------------
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);
	//--------------------------------------------------------

}

void Object3D::UpdateViewmatrix()
{
	//ビュー行列の更新
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);
}

void Object3D::Initialize()
{
	HRESULT result = S_FALSE;

	//定数バッファの作成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
}

void Object3D::Update()
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	//スケール・回転・移動行列の計算------------------------------------
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//------------------------------------------------------------------

	//ワールド行列の合成--------------------------------
	matWorld = XMMatrixIdentity();//変形をリセット
	matWorld *= matScale;//ワールド行列にスケールを反映
	matWorld *= matRot;	 //ワールド行列に回転を反映
	matWorld *= matTrans;//ワールド行列に移動を反映
	//--------------------------------------------------

	//定数バッファへデータ転送----------------------------
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->mat = matWorld * matView * matProjection;
	constBuff->Unmap(0, nullptr);
	//----------------------------------------------------

}

void Object3D::Draw()
{
	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	//ディスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(
		0,
		constBuff->GetGPUVirtualAddress()
	);
	//シェーダーリソースビューのセット
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		gpuDescHandleSRV
	);

	//描画コマンド
	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}
