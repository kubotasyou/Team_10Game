#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;
using namespace DirectX;

//3D�I�u�W�F�N�g�N���X
class Object3D
{
public://�T�u�N���X

	//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;   //xyz���W
		XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv;	//uv���W
	};

	//�萔�o�b�t�@�]���f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color;//�F
		XMMATRIX mat;//���[���h�ϊ��s��
	};

private://�萔
	static const int divison = 50;  //������
	static const float radius;      //���a
	static const float prizmHeight; //����
	static const int planeCount
		= divison * 2 + divison * 2;//�ʂ̐�
	static const int vertexCount = planeCount * 3;//���_��

public://�ÓI�����o�֐�(render)

	//�ÓI������(�f�o�C�X�E��ʕ��E��ʍ���)
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//�ÓI�`��O����(�R�}���h���X�g)
	static void BeginDraw(ID3D12GraphicsCommandList* cmdList);

	//�ÓI�`��㏈��
	static void EndDraw();

	//�ÓI3D�I�u�W�F�N�g����
	static Object3D* Create();

	//�ÓI���_���W�̎擾
	static const XMFLOAT3& GetEye()
	{
		return eye;
	}

	//�ÓI���_���W�̐ݒ�(���W)
	static void SetEye(XMFLOAT3 eyePos);

	//�ÓI�����_���W�̎擾
	static const XMFLOAT3& GetTarget()
	{
		return target;
	}

	//�ÓI�����_���W�̐ݒ�(���W)
	static void SetTarget(XMFLOAT3 targetPos);

	//�ÓI�x�N�g���ɂ��ړ�(�ړ���)
	static void CameraMoveVector(XMFLOAT3 velocity);

private://�ÓI�����o�ϐ�(render)

	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	static ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	static D3D12_INDEX_BUFFER_VIEW ibView;

	// ���_�f�[�^�z��
	static Vertex vertices[vertexCount];
	// ���_�C���f�b�N�X�z��
	static unsigned short indices[planeCount * 3];

private://�ÓI�����o�֐�??????

	//�f�B�X�N���v�^�q�[�v�̏�����
	static void InitializeDescriptorHeap();

	//�J����������(��ʕ��E��ʍ���)
	static void InitializeCamera(int window_width, int window_height);

	//�O���t�B�N�X�p�C�v���C������
	static void InitializeGraphicsPipeline();

	//�e�N�X�`���ǂݍ���
	static void Loadtexture();

	//���f���쐬
	static void CreateModel();

	//�r���[�s����X�V
	static void UpdateViewmatrix();

public://�����o�֐�

	void Initialize();

	void Update();

	void Draw();

	//���W�擾
	const XMFLOAT3& GetPosition()
	{
		return position;
	}

	//���W�̐ݒ�
	void SetPosition(XMFLOAT3 position)
	{
		this->position = position;
	}

private://�����o�ϐ�

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�F
	XMFLOAT4 color = { 1,1,1,1 };
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld;
	//�e�I�u�W�F�N�g(?)
	Object3D* parent = nullptr;

};

