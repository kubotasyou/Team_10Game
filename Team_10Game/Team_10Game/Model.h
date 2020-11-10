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

public://�T�u�N���X

	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;   //xyz���W
		XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv;    //uv���W
	};

	//�}�e���A��
	struct Material
	{
		std::string name;			//�}�e���A����
		XMFLOAT3 ambient;			//�A���r�G���g�̉e���x
		XMFLOAT3 diffuse;			//�f�B�t���[�Y�̉e���x
		XMFLOAT3 specular;			//�X�y�L�����[�̉e���x
		float alpha;      			//���l
		std::string textureFilename;//�e�N�X�`���t�@�C����

		//�R���X�g
		Material()
		{
			//������
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public://public�֐�
	
	//�R���X�g���N�^(�f�o�C�X)
	Model(ID3D12Device* device);

	//���f������(���f����)
	void CreateModel(const std::string& fileName);

	//�`��(�R�}���h���X�g)
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//�}�e���A���󂯓n��
	Material* GetMaterial()
	{
		return material;
	}

private://private�֐�

	//�}�e���A���̓ǂݍ���(�t�@�C�����E.�}�e���A���t�@�C����)
	void LoadMaterial(const std::string& directoryName, const std::string& fileName);

	//�e�N�X�`���̓ǂݍ���(�t�@�C�����E.�e�N�X�`���t�@�C����)
	void LoadTexture(const std::string& directoryName, const std::string& fileName);

private://�ϐ�

	//�}�e���A������
	Material* material;

	ID3D12Device* device;                          //�f�o�C�X�󂯎��p
	ComPtr<ID3D12Resource> vertBuff;               //���_�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;              //�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> texbuff;                //�e�N�X�`���o�b�t�@
	ComPtr<ID3D12DescriptorHeap> descHeap;         //�f�X�N���v�^�q�[�v
	UINT descriptorHandleIncrementSize;            //�q�[�v�T�C�Y�����p
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;//cpu�̐擪�n���h���擾�p
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;//gpu�̐擪�n���h���擾�p
	D3D12_VERTEX_BUFFER_VIEW vbView;               //���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;                //�C���f�b�N�X�o�b�t�@�r���[
	std::vector<VertexPosNormalUv> vertices;       //���_�z��
	std::vector<unsigned short> indices;           //�C���f�b�N�X�z��
};

