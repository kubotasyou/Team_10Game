#pragma once
#include <Windows.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>

/// <summary>
/// ���C�g
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
		XMVECTOR lightv;    //���C�g�ւ̕�����\���x�N�g��
		XMFLOAT3 lightcolor;//���C�g�̐F
	};

private:
	static ID3D12Device* device;

public:
	///<summary>
	///�ÓI������
	///</summary>
	///<param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	///<summary>
	///�C���X�^���X����
	///</summary>
	///<returns>�C���X�^���X</returns>
	static Light* Create();

	///<summary>
	///������
	///</summary>
	void Initialize();

	///<summary>
	///�萔�o�b�t�@�̓]��
	///</summary>
	void TransferConstBuffer();

	///<summary>
	///���C�g�̕������Z�b�g
	///</summary>
	///<param name="lightdir">���C�g����</param>
	void SetLightDir(const XMVECTOR& lightdir);

	///<summary>
	///���C�g�F���Z�b�g
	///</summary>
	///<param name="lightcolor">���C�g�F</param>
	void SetLightColor(const XMFLOAT3& lightcolor);

	///<summary>
	///�X�V
	///</summary>
	void Updata();

	///<summary>
	///�`��
	///</summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

private:
	ComPtr<ID3D12Resource> constBuff;//�萔�o�b�t�@
	XMVECTOR lightdir = { 1,0,0,0 };//���C�g�̌��������i�P�ʃx�N�g���j
	XMFLOAT3 lightcolor = { 1,1,1 };//���C�g�F
	bool dirty = false;
};