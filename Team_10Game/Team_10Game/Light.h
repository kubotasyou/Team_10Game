#pragma once
#include <Windows.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>

/// <summary>
/// ライト
/// </summary>
class Light
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ConstBufferData
	{
		XMVECTOR lightv;    //ライトへの方向を表すベクトル
		XMFLOAT3 lightcolor;//ライトの色
	};

private:
	static ID3D12Device* device;

public:
	///<summary>
	///静的初期化
	///</summary>
	///<param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	///<summary>
	///インスタンス生成
	///</summary>
	///<returns>インスタンス</returns>
	static Light* Create();

	///<summary>
	///初期化
	///</summary>
	void Initialize();

	///<summary>
	///定数バッファの転送
	///</summary>
	void TransferConstBuffer();

	///<summary>
	///ライトの方向をセット
	///</summary>
	///<param name="lightdir">ライト方向</param>
	void SetLightDir(const XMVECTOR& lightdir);

	///<summary>
	///ライト色をセット
	///</summary>
	///<param name="lightcolor">ライト色</param>
	void SetLightColor(const XMFLOAT3& lightcolor);

	///<summary>
	///更新
	///</summary>
	void Updata();

	///<summary>
	///描画
	///</summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

private:
	ComPtr<ID3D12Resource> constBuff;//定数バッファ
	XMVECTOR lightdir = { 1,0,0,0 };//ライトの光線方向（単位ベクトル）
	XMFLOAT3 lightcolor = { 1,1,1 };//ライト色
	bool dirty = false;
};