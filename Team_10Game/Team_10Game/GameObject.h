#pragma once
#include "Model.h"

class GameObject
{
private:// Microsoft::WRL::���ȗ�
	
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�T�u�N���X

	//�萔�o�b�t�@�\����B0
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// �R�c�ϊ��s��
		XMFLOAT4 color;
	};

	//�萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; //�A���r�G���g
		float pad1;		  //�p�e�B���O
		XMFLOAT3 diffuse; //�f�B�t���[�Y
		float pad2;		  //�p�e�B���O
		XMFLOAT3 specular;//�X�y�L�����[
		float alpha;      //�p�e�B���O
	};

public://��񂵂��ǂ񂶂Ⴂ���Ȃ����

	//ReadMe : static���C�ɂȂ邩��g��Ȃ����@���l����

	//��x�����̏�����(�f�o�C�X�E��ʕ��E��ʍ���)
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//�`��O����(�R�}���h���X�g)
	static void BeginDraw(ID3D12GraphicsCommandList* cmdList);

	//�`��㏈��
	static void EndDraw();

	//ReadMe : 3D�I�u�W�F�N�g������static���g��Ȃ��ō��
	static GameObject* Create();

	//���_���W�ݒ�
	static void SetEye(XMFLOAT3 eye);

	//�����_���W�ݒ�
	static void SetTarget(XMFLOAT3 target);

	//���_���W�擾
	static const XMFLOAT3& GetEye()
	{
		return eye;
	}

	//�����_���W�擾
	static const XMFLOAT3& GetTarget()
	{
		return target;
	}

	//�x�N�g���ɂ��J�����̈ړ�
	static void CameraMoveVector(XMFLOAT3 velocity);

	//�J�����̈ړ�(�^�[�Q�b�g�ύX�Ȃ�)
	static void CameraRotVector(XMFLOAT3 rotation);

private://��񂵂��ǂ񂶂Ⴂ���Ȃ����part2

	//�f�X�N���v�^�q�[�v�̏�����
	static void InitializeDescriptorHeap();

	//�J�����̏�����(��ʕ��E��ʍ���)
	static void InitializeCamera(int winodw_width, int window_height);

	//�O���t�B�N�X�p�C�v���C���̏�����
	static void InitializeGraphicsPipeline();

	//�r���[�s����X�V
	static void UpdateViewMatrix();

public://�֐�

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	//���f���̃Z�b�g
	void SetModel(Model* _model);

	//���W�̐ݒ�
	void SetPosition(XMFLOAT3 position)
	{
		this->position = position;
	}

	//�F�̐ݒ�
	void SetColor(XMFLOAT4 color)
	{
		this->color = color;
	}

	//�傫���̐ݒ�
	void SetScale(XMFLOAT3 scale)
	{
		this->scale = scale;
	}

	//���W�̎擾
	const XMFLOAT3& GetPosition()
	{
		return position;
	}

	//���a�̐ݒ�
	void SetRadius(float radius)
	{
		this->radius = radius;
	}

	//���a�擾
	const float& GetRadius()
	{
		return radius;
	}

private://��x�����Ă΂Ȃ������o�ϐ�

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

private://�����o�ϐ�

	Model* model = nullptr;

	ComPtr<ID3D12Resource> constBuffB0;//�萔�o�b�t�@B0
	ComPtr<ID3D12Resource> constBuffB1;//�萔�o�b�t�@B1
		
	XMFLOAT4 color = { 1,1,1,1 }; //�F
	XMFLOAT3 scale = { 1,1,1 };   //���[�J���X�P�[��	
	XMFLOAT3 rotation = { 0,0,0 };//X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 position = { 0,0,0 };//���[�J�����W
	XMMATRIX matWorld;            //���[�J�����[���h�ϊ��s��

	//���a
	float radius = 1.0f;

};

