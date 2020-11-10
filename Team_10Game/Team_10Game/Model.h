#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

using namespace DirectX;

class Model
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://サブクラス

	//頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;   //xyz座標
		XMFLOAT3 normal;//法線ベクトル
		XMFLOAT2 uv;    //uv座標
	};

	//マテリアル
	struct Material
	{
		std::string name;			//マテリアル名
		XMFLOAT3 ambient;			//アンビエントの影響度
		XMFLOAT3 diffuse;			//ディフューズの影響度
		XMFLOAT3 specular;			//スペキュラーの影響度
		float alpha;      			//α値
		std::string textureFilename;//テクスチャファイル名

		//コンスト
		Material()
		{
			//初期化
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public://public関数
	
	//コンストラクタ(デバイス)
	Model(ID3D12Device* device);

	//モデル生成(モデル名)
	void CreateModel(const std::string& fileName);

	//描画(コマンドリスト)
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//マテリアル受け渡し
	Material* GetMaterial()
	{
		return material;
	}

private://private関数

	//マテリアルの読み込み(ファイル名・.マテリアルファイル名)
	void LoadMaterial(const std::string& directoryName, const std::string& fileName);

	//テクスチャの読み込み(ファイル名・.テクスチャファイル名)
	void LoadTexture(const std::string& directoryName, const std::string& fileName);

private://変数

	//マテリアル生成
	Material* material;

	ID3D12Device* device;                          //デバイス受け取り用
	ComPtr<ID3D12Resource> vertBuff;               //頂点バッファ
	ComPtr<ID3D12Resource> indexBuff;              //インデックスバッファ
	ComPtr<ID3D12Resource> texbuff;                //テクスチャバッファ
	ComPtr<ID3D12DescriptorHeap> descHeap;         //デスクリプタヒープ
	UINT descriptorHandleIncrementSize;            //ヒープサイズ所得用
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;//cpuの先頭ハンドル取得用
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;//gpuの先頭ハンドル取得用
	D3D12_VERTEX_BUFFER_VIEW vbView;               //頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;                //インデックスバッファビュー
	std::vector<VertexPosNormalUv> vertices;       //頂点配列
	std::vector<unsigned short> indices;           //インデックス配列
};

