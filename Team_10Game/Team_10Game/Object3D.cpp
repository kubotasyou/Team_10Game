#include "Object3D.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma region �ÓI�����o�ϐ��̎���

const float Object3D::radius = 5.0f;				// ��ʂ̔��a
const float Object3D::prizmHeight = 8.0f;			// ���̍���
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

//��x�����Ăяo����Ȃ�������
void Object3D::StaticInitialize(ID3D12Device * dev, int window_width, int window_height)
{
	//�f�o�C�X���Z�b�g
	Object3D::device = dev;

	//�f�B�X�N���v�^�q�[�v�̏�����
	InitializeDescriptorHeap();

	//�J����������
	InitializeCamera(window_width, window_height);

	//�O���t�B�N�X�p�C�v���C��������
	InitializeGraphicsPipeline();

	//�e�N�X�`���ǂݍ���
	Loadtexture();

	//���f������
	CreateModel();
}

//��x�����Ăяo����Ȃ��`��O����
void Object3D::BeginDraw(ID3D12GraphicsCommandList * cmdList)
{
	// �R�}���h���X�g���Z�b�g
	Object3D::cmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//��x�����Ăяo����Ȃ��`��㏈��
void Object3D::EndDraw()
{
	//�R�}���h���X�g������
	Object3D::cmdList = nullptr;
}

Object3D * Object3D::Create()
{
	//3D�I�u�W�F�N�g�C���X�^���X�𐶐�
	Object3D* object3d = new Object3D();

	//������
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

	//�f�B�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//�V�F�[�_�[�t�@�C������m�F�ł���悤��
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//�V�F�[�_�[���\�[�X�r���[�͈��
	descHeapDesc.NumDescriptors = 1;

	//����
	result = device->CreateDescriptorHeap(
		&descHeapDesc,
		IID_PPV_ARGS(&descHeap)
	);

	//�f�B�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize =
		device->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);
}

void Object3D::InitializeCamera(int window_width, int window_height)
{
	//�r���[�s��̐���
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);

	//�������e�ɂ��ˉe�ϊ��s��
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

	//�V�F�[�_�[�I�u�W�F�N�g����
	ComPtr<ID3DBlob> vsBlob;   //���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;   //�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;//�G���[�I�u�W�F�N�g

#pragma region �e�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��

	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVertexShader.hlsl",                     //�V�F�[�_�[��
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,              //�C���N���[�h���\�ɂ���
		"VSmain",                                       //�G���g���[�|�C���g�̎w��
		"vs_5_0",                                       //�V�F�[�_�[���f���̎w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob
	);
	//�ǂݎ��G���[���̏���
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPixelShader.hlsl",                     //�V�F�[�_�[��
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,              //�C���N���[�h���\�ɂ���
		"PSmain",                                       //�G���g���[�|�C���g�̎w��
		"ps_5_0",                                       //�V�F�[�_�[���f���̎w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob
	);
	//�ǂݎ��G���[���̏���
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion

#pragma region ���_���C�A�E�g�ݒ�

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{//xyz���W
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//�@���x�N�g��
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0

		},
		{//uv���W
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

#pragma region �O���t�B�N�X�p�C�v���C���̍쐬

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//�V�F�[�_�[�̃Z�b�g---------------------------------
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	//---------------------------------------------------

	//�T���v���}�X�N(�W���ݒ�)
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//���X�^���C�Y�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//�w�ʃJ�����O�����Ȃ�
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//��ɏ㏑���ł���ݒ�ɂ���
	//gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	//�u�����h�̐ݒ�--------------------------------------------------
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	//RGBA�S�Ẵ`�����l����`��(�W���ݒ�)
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//���ʐݒ�
	blendDesc.BlendEnable = true;//�u�����h��L���ɂ���
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z����
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃��l
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//�f�X�g�̃��l

	//���Z�����̐ݒ�
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;  //�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

	//�u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blendDesc;
	//----------------------------------------------------------------

	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//���_���C�A�E�g�̐ݒ�------------------------------------
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	//--------------------------------------------------------

	//�}�`�̌`��̐ݒ�
	gpipeline.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//�`��Ώۂ̐�
	gpipeline.NumRenderTargets = 1;

	//0�`1�ɐ��K�����ꂽRGBA
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	//1�s�N�Z���ɂ�1��̃T���v�����O
	gpipeline.SampleDesc.Count = 1;

	//�f�B�X�N���v�^�����W------------------------------------
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	//t0���W�X�^
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	//--------------------------------------------------------

	//���[�g�p�����[�^�[----------------------------------
	CD3DX12_ROOT_PARAMETER rootparams[2];

	rootparams[0].InitAsConstantBufferView(0, 0,
		D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV,
		D3D12_SHADER_VISIBILITY_ALL);
	//----------------------------------------------------

	//�X�^�e�B�b�N�T���v���[(s0���W�X�^)
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc =
		CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�`���̐ݒ�------------------------------------------
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams),
		rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//----------------------------------------------------------------

	ComPtr<ID3DBlob> rootSigBlob;
	//�o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob
	);

	//���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0,
		rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootsignature)
	);

	//���[�g�V�O�l�`���̐ݒ�𔽉f
	gpipeline.pRootSignature = rootsignature.Get();

	//�O���t�B�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(&pipelinestate)
	);

#pragma endregion

}

void Object3D::Loadtexture()
{
	HRESULT result = S_FALSE;

#pragma region �e�N�X�`���̓ǂݍ���

	//�摜�̃T�C�Y�ƃe�N�X�`���Ƃ��Ă̏�񂪓���B
	TexMetadata metadata{};
	//�摜�̃f�[�^�̃A�h���X������B
	ScratchImage scratchImg{};

	//�e�N�X�`���̓ǂݍ���
	result = LoadFromWICFile(
		L"Resources/Texture/tex1.png",
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	//���f�[�^�̒��o
	const Image* img = scratchImg.GetImage(0, 0, 0);

	//���\�[�X�̐ݒ�
	//�ǂݍ��񂾃f�[�^�����e�N�X�`���f�[�^�ɐݒ肷��
	CD3DX12_RESOURCE_DESC texresDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			static_cast<UINT>(metadata.height),
			static_cast<UINT16>(metadata.arraySize),
			static_cast<UINT16>(metadata.mipLevels)
		);

	//�e�N�X�`���o�b�t�@�̍쐬
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);

	//�e�N�X�`���o�b�t�@�Ƀf�[�^��]��
	result = texbuff->WriteToSubresource(
		0,
		nullptr,             //�S�̈�ɃR�s�[
		img->pixels,         //���̃f�[�^�A�h���X
		static_cast<UINT>(img->rowPitch), //1���C���T�C�Y
		static_cast<UINT>(img->slicePitch)//1���T�C�Y
	);

#pragma endregion

#pragma region �V�F�[�_�[���\�[�X�r���[�쐬

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

	//�ݒ�\����
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//2D�e�N�X�`��
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//����
	device->CreateShaderResourceView(texbuff.Get(),
		&srvDesc,//�e�N�X�`���ݒ���
		cpuDescHandleSRV
	);

#pragma endregion
}

void Object3D::CreateModel()
{
	HRESULT result = S_FALSE;

	std::vector<Vertex> realVertices;

#pragma region ���_���W�̌v�Z(�d������)
	{
		realVertices.resize((divison + 1) * 2);
		int index = 0;
		float zValue;

		//���
		zValue = prizmHeight / 2.0f;
		for (int i = 0; i < divison; i++)
		{
			XMFLOAT3 vertex;
			vertex.x = radius * sinf(XM_2PI / divison * i);
			vertex.y = radius * cosf(XM_2PI / divison * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		//��ʂ̒��S�_
		realVertices[index++].pos = XMFLOAT3(0, 0, zValue);

		//�V��
		zValue = -prizmHeight / 2.0f;
		for (int i = 0; i < divison; i++)
		{
			XMFLOAT3 vertex;
			vertex.x = radius * sinf(XM_2PI / divison * i);
			vertex.y = radius * cosf(XM_2PI / divison * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		//�V�ʂ̒��S�_
		realVertices[index++].pos = XMFLOAT3(0, 0, zValue);
	}

#pragma endregion

#pragma region ���_�̌v�Z(�d������)
	
	{
		int index = 0;
		//���
		for (int i = 0; i < divison; i++)
		{
			unsigned short index0 = i + 1;
			unsigned short index1 = i;
			unsigned short index2 = divison;

			vertices[index++] = realVertices[index0];
			vertices[index++] = realVertices[index1];
			vertices[index++] = realVertices[index2];//��ʂ̒��S�_
		}
		//��ʂ̍Ō�̎O�p�`�̈�Ԗڂ̃C���f�b�N�X��0�ɏ����ւ�
		vertices[index - 3] = realVertices[0];

		int topStart = divison + 1;
		//�V��
		for (int i = 0; i < divison; i++)
		{
			unsigned short index0 = topStart + i;
			unsigned short index1 = topStart + i + 1;
			unsigned short index2 = topStart + divison;

			vertices[index++] = realVertices[index0];
			vertices[index++] = realVertices[index1];
			vertices[index++] = realVertices[index2];//�V�ʂ̒��S�_
		}
		//�V�ʂ̍Ō�̎O�p�`��1�Ԗڂ̃C���f�b�N�X��0�ɏ����ւ�
		vertices[index - 2] = realVertices[topStart];

		//����
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

#pragma region ���_�C���f�b�N�X�̐ݒ�

	//���_�C���f�b�N�X�̐ݒ�
	for (int i = 0; i < _countof(indices); i++)
	{
		indices[i] = i;
	}

#pragma endregion

#pragma region �@�������̌v�Z

	for (int i = 0; i < _countof(indices) / 3; i++)
	{//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X���擾
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g���v�Z
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g���ŊO��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^��normal�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}

#pragma endregion

	//���_�o�b�t�@����-----------------------------------
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	//---------------------------------------------------

	//�C���f�b�N�X�o�b�t�@����---------------------------
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);
	//---------------------------------------------------

	//���_�o�b�t�@�փf�[�^�]��----------------------------
	Vertex* vertMap = nullptr;
	
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	memcpy(vertMap, vertices, sizeof(vertices));

	vertBuff->Unmap(0, nullptr);
	//----------------------------------------------------

	//�C���f�N�X�o�b�t�@�փf�[�^�]��-----------------------
	unsigned short* indexMap = nullptr;

	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	//�S�C���f�b�N�X�ɂ���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}

	indexBuff->Unmap(0, nullptr);
	//-----------------------------------------------------

	//���_�o�b�t�@�r���[�̍쐬-------------------------------
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
	//-------------------------------------------------------

	//�C���f�N�X�o�b�t�@�r���[�̍쐬--------------------------
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);
	//--------------------------------------------------------

}

void Object3D::UpdateViewmatrix()
{
	//�r���[�s��̍X�V
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);
}

void Object3D::Initialize()
{
	HRESULT result = S_FALSE;

	//�萔�o�b�t�@�̍쐬
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

	//�X�P�[���E��]�E�ړ��s��̌v�Z------------------------------------
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//------------------------------------------------------------------

	//���[���h�s��̍���--------------------------------
	matWorld = XMMatrixIdentity();//�ό`�����Z�b�g
	matWorld *= matScale;//���[���h�s��ɃX�P�[���𔽉f
	matWorld *= matRot;	 //���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;//���[���h�s��Ɉړ��𔽉f
	//--------------------------------------------------

	//�萔�o�b�t�@�փf�[�^�]��----------------------------
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->mat = matWorld * matView * matProjection;
	constBuff->Unmap(0, nullptr);
	//----------------------------------------------------

}

void Object3D::Draw()
{
	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	//�f�B�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(
		0,
		constBuff->GetGPUVirtualAddress()
	);
	//�V�F�[�_�[���\�[�X�r���[�̃Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		gpuDescHandleSRV
	);

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}
