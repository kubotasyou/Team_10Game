#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class Sprite
{
public://�V�F�[�_�ɑ���f�[�^

	//���_�f�[�^
	struct VertexData
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT2 uv; //uv���W
	};

	//�萔�f�[�^
	struct ConstBufferData
	{
		XMFLOAT4 color;//�F
		XMMATRIX mat;  //3D�ϊ��s��
	};

public://static ���ʂ̃����o�֐�

	//���ʏ�����(�f�o�C�X�E��ʕ��E��ʍ���)
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//�e�N�X�`���ǂݍ���(�e�N�X�`���ԍ��E�摜�t�@�C����)
	static void LoadTexture(UINT texNumber, const wchar_t* fileName);

	//�`��J�n����(�`��R�}���h���X�g)
	static void BeginDraw(ID3D12GraphicsCommandList* cmdList);

	//�`��I������
	static void EndDraw();

	//�X�v���C�g����(�������ꂽ�X�v���C�g��Ԃ�)
	//(�e�N�X�`���ԍ��E���W�E�F�E�A���J�[�|�C���g�E���E���]�E�㉺���])
	static Sprite* Create(UINT texNumber, XMFLOAT2 position,
		XMFLOAT4 color = { 1,1,1,1 }, XMFLOAT2 anchorPoint = {0.0f,0.0f},
		bool isFlipX = false, bool isFlipY = false);

private://static �ϐ�

	//�e�N�X�`���̍ő喇��(SRV�̐�?)
	static const int srvCount = 512;

	//���_��(�l�p�`)
	static const int vertNum = 4;

	//�f�o�C�X
	static ID3D12Device* device;

	//�f�B�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;

	//�`��R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;

	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;

	//�p�C�v���C���X�e�[�g
	static ComPtr<ID3D12PipelineState> pipelineState;

	//�ˉe�ϊ��s��
	static XMMATRIX matProjection;

	//�f�B�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;

	//�e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff[srvCount];

public://�����o�֐�

	//�R���X�g
	//(�e�N�X�`���ԍ��E���W�E�T�C�Y�E�F�E�A���J�[�|�C���g
	// ���E���]�E�㉺���])
	Sprite(UINT texNumber, XMFLOAT2 position,
		XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorPoint,
		bool isFlipX, bool isFlipY);

	//������
	void Initialize();

	//��]�p�ݒ�(�p�x)
	void SetRotation(float rotation);

	//���W�ݒ�(�ʒu)
	void SetPosition(XMFLOAT2 position);

	//�T�C�Y�ݒ�(�傫��)
	void SetScale(XMFLOAT2 size);

	//�F�̕ύX(�F)
	void SetColor(XMFLOAT4 color);

	//�A���J�[�|�C���g�̐ݒ�(���S�_)
	void SetAnchorPoint(XMFLOAT2 anchorPoint);

	//���E���]�̐ݒ�(���E���]���邩�ǂ���)
	void SetFlitX(bool isFlipX);

	//�㉺���]�̐ݒ�(�㉺���]���邩�ǂ���)
	void SetFlitY(bool isFlipY);

	//�e�N�X�`���͈͎w��(�e�N�X�`���̍�����W�E�e�N�X�`���T�C�Y)
	void SetTextureRect(XMFLOAT2 texBasePos, XMFLOAT2 texSize);

	//�`��
	void Draw();

private://�����o�ϐ�

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���_�o�b�t�@�[�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//�e�N�X�`���ԍ�
	UINT texNumber = 0;
	//Z������̉�]�p
	float rotationZ = 0.0f;
	//���W
	XMFLOAT2 position{};
	//�X�v���C�g�̕��A����(�����l)
	XMFLOAT2 size = { 100.0f,100.0f };
	//�A���J�[�|�C���g(���S�_)
	XMFLOAT2 anchorPoint = { 0,0 };
	//���[���h�s��
	XMMATRIX matWorld{};
	//�F
	XMFLOAT4 color = { 1,1,1,1 };
	//���E���]
	bool isFlipX = false;
	//�㉺���]
	bool isFlipY = false;

	//�e�N�X�`���n�_
	XMFLOAT2 texBasePos = { 0,0 };
	//�e�N�X�`���̕��A����
	XMFLOAT2 texSize = { 100.0f,100.0f };

private://�����o�֐�

	//���_�f�[�^�]��
	void SendVertexData();

};
