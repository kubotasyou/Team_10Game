#include "Model.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string> 
#include <vector> 

#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

Model::Model(ID3D12Device * device)
	:device(device)
{
	HRESULT result = S_FALSE;

	//ReadMe : デスクリプタヒープは最初に一回作ってるからそれを引数で受け取ればいいのでは?

	//デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 512;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	//デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Model::CreateModel(const std::string & path)
{
	HRESULT result = S_FALSE;

	//ファイルストリーム
	std::ifstream file;

	//2-2ファイル読み込み(文字列を変えるだけで読み込めるようにする。)
	const string modelName = path;
	const string fileName = modelName + ".obj";
	const string directoryName = "Resources/" + modelName + "/";
	file.open(directoryName + fileName);

	//ファイル読み込み時失敗かをチェック
	if (file.fail())
	{
		//警告を出すやつ
		assert(0);
	}

	vector<XMFLOAT3> positions;//頂点座標
	vector<XMFLOAT3> normals;  //法線ベクトル
	vector<XMFLOAT2> texcoords;//uv座標

	//一行ずつ読み取る
	string line;

#pragma region ファイルの読み込み

	while (std::getline(file, line))
	{
		//一行分の文字列(line)を、ストリーム変換(解析)する
		std::istringstream line_stream(line);

		//半角スペースで区切って、行の先頭文字列(vとかf)を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字が'mtllib'ならマテリアル
		if (key == "mtllib")
		{
			//マテリアルファイルの読み込み
			string fileName;

			line_stream >> fileName;

			//マテリアルの読み込み
			LoadMaterial(directoryName, fileName);
		}

		//先頭文字列が'v'なら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標の読み込み
			XMFLOAT3 position{};

			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//座標データに追加
			positions.emplace_back(position);

		}

		//先頭文字列が'vt'ならテクスチャ(uv座標)
		if (key == "vt")
		{
			//UV成分読み込み
			XMFLOAT2 texcoord{};

			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			//v方向反転
			texcoord.y = 1.0f - texcoord.y;
			//↑y軸が上下逆の場合があるため。

			//テクスチャ座標のvectorデータに追加
			texcoords.emplace_back(texcoord);
		}

		//先頭文字列が'vn'なら法線ベクトル
		if (key == "vn")
		{
			//X,Y,Z成分読み込み
			XMFLOAT3 normal{};

			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			//法線ベクトルのvectorデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列が'f'ならポリゴン(三角形)
		if (key == "f")
		{
			int faceIndexCount = 0;

			//半角スペースで区切って、行の続きを読み込む
			string index_string;

			while (std::getline(line_stream, index_string, ' '))
			{
				//頂点インデックス１個分の文字列を
				//ストリームに変換して、解析しやすくする。
				std::istringstream index_stream(index_string);

				//頂点インデックス読み込み
				unsigned short indexPosition, indexNormal, indexTexcoord;

				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexNormal;

				// 2-1 追加
				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				//頂点インデックスに追加
				//.objファイルでは、インデックスが1始まりなので
				//-1して、0スタートに合わせるようにする。
				

				//一行の読み込み数が3より多かったら
				if (faceIndexCount >= 3)
				{
					indices.emplace_back(indices.size() - 1);
					indices.emplace_back(indices.size() - 1);
					indices.emplace_back(indices.size() - 5);
				}
				else
				{
					indices.emplace_back(indices.size());
				}
				faceIndexCount++;
			}
		}
	}

#pragma endregion

	//忘れずにファイルを閉じる
	file.close();

	//頂点の要素数を入れておく。
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv)* vertices.size());
	//インデックスの要素数を入れる。
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)*indices.size());

#pragma region 各バッファの作成・転送・ビュー生成

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB/*sizeof(vertices)*/),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB/*sizeof(indices)*/),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		std::copy(indices.begin(), indices.end(), indexMap);

		indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

}

void Model::Draw(ID3D12GraphicsCommandList * cmdList)
{
	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	//デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);

	//描画
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Model::LoadMaterial(const std::string & directoryName, const std::string & fileName)
{
	//ファイルストリーム
	std::ifstream file;

	material = new Material();

	//マテリアルファイルを開く
	file.open(directoryName + fileName);

	//ファイルオープンの合否をチェック
	if (file.fail())
	{
		assert(0 + "ファイルオープンに失敗");
	}

	//1行ずつ読み込む
	string line;

	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		//半角スペース区切りで、行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のTab文字は無視する
		if (key[0] == '\t')
		{
			//先頭の文字を削除
			key.erase(key.begin());
		}

		//先頭文字列が'newmtl'ならマテリアル名
		if (key == "newmtl")
		{
			//マテリアル名読み込み
			line_stream >> material->name;
		}

		//先頭文字列が'Ka'ならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}

		//先頭文字列が'Kd'ならディフューズ色
		if (key == "Kd")
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}

		//先頭文字列が'Ks'ならスペキュラー色
		if (key == "Ks")
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}

		//先頭文字列が'map_Kd'ならテクスチャファイル名色
		if (key == "map_Kd")
		{
			//テクスチャファイル名読み込み
			line_stream >> material->textureFilename;

			//テクスチャ読み込み
			LoadTexture(directoryName, material->textureFilename);
		}
	}

	//ファイルを閉じる
	file.close();
}

void Model::LoadTexture(const std::string & directoryName, const std::string & fileName)
{
	HRESULT result = S_FALSE;

	//ファイルパスを結合
	string filepath = directoryName + fileName;

	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(
		CP_ACP,
		0,
		filepath.c_str(),
		-1,
		wfilepath,
		_countof(wfilepath)
	);

	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0); //生データ抽出

	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels)
	);

	//テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);

	//テクスチャバッファにデータ転送
	result = texbuff->WriteToSubresource(
		0,
		nullptr,                          //全領域へコピー
		img->pixels,                      //元データアドレス
		static_cast<UINT>(img->rowPitch), //1ラインサイズ
		static_cast<UINT>(img->slicePitch)//1枚サイズ
	);

	//ReadMe : シェーダーリソースビューって各オブジェクトに一つ必要なんだっけ?

	//シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff.Get(),//ビューと関連付けるバッファ
		&srvDesc,                                  //テクスチャ設定情報
		cpuDescHandleSRV
	);

}
