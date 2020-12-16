#include "Sprite.h"

#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//�ÓI�����o�ϐ�
ID3D12Device* Sprite::device = nullptr;
UINT Sprite::descriptorHandleIncrementSize;
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Sprite::rootSignature;
ComPtr<ID3D12PipelineState> Sprite::pipelineState;
XMMATRIX Sprite::matProjection;
ComPtr<ID3D12DescriptorHeap> Sprite::descHeap;
ComPtr<ID3D12Resource> Sprite::texBuff[srvCount];

void Sprite::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	//�����̎󂯎��
	Sprite::device = device;

	//�f�B�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize =
		device->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);

	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob> vsBlob;//���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;//�G���[�I�u�W�F�N�g

#pragma region �V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��

	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"SpriteVertexShader.hlsl",                     //�V�F�[�_�[��
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
		L"SpritePixelShader.hlsl",                     //�V�F�[�_�[��
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
			0,
        },
	    {//uv���W
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0,
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
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//��ɏ㏑���ł���ݒ�ɂ���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

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
		CD3DX12_STATIC_SAMPLER_DESC(0,
			D3D12_FILTER_MIN_MAG_MIP_POINT);

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
		IID_PPV_ARGS(&rootSignature)
	);

	//���[�g�V�O�l�`���̐ݒ�𔽉f
	gpipeline.pRootSignature = rootSignature.Get();

	//�O���t�B�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline,
		IID_PPV_ARGS(&pipelineState)
	);

#pragma endregion

	//�f�B�X�N���v�^�q�[�v�𐶐�-----------------------------------
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//�V�F�[�_�[���猩����悤�ɂ���B
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = srvCount;
	result = device->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(&descHeap)
	);
	//-------------------------------------------------------------

	//�ˉe�s��v�Z
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f,
		(float)window_width,
		(float)window_height,
		0.0f, 0.0f, 1.0f
	);

}

void Sprite::LoadTexture(UINT texNumber, const wchar_t * fileName)
{
	if (device == nullptr)
	{
		assert("�f�o�C�X��");
	}

	HRESULT result = S_FALSE;

#pragma region �e�N�X�`���̓ǂݍ���

	//�摜�̃T�C�Y�ƃe�N�X�`���Ƃ��Ă̏�񂪓���B
	TexMetadata metadata{};
	//�摜�̃f�[�^�̃A�h���X������B
	ScratchImage scratchImg{};

	//�e�N�X�`���̓ǂݍ���
	result = LoadFromWICFile(
		fileName,
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
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels
		);

	//�e�N�X�`���o�b�t�@�̍쐬
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texBuff[texNumber])
	);

	//�e�N�X�`���o�b�t�@�Ƀf�[�^��]��
	result = texBuff[texNumber]->WriteToSubresource(
		0,
		nullptr,             //�S�̈�ɃR�s�[
		img->pixels,         //���̃f�[�^�A�h���X
		(UINT)img->rowPitch, //1���C���T�C�Y
		(UINT)img->slicePitch//1���T�C�Y
	);

#pragma endregion

#pragma region �V�F�[�_�[���\�[�X�r���[�쐬

	//�ݒ�\����
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//2D�e�N�X�`��
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//����
	device->CreateShaderResourceView(texBuff[texNumber].Get(),
		&srvDesc,//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(),
			texNumber,descriptorHandleIncrementSize)
	);

#pragma endregion

}

void Sprite::BeginDraw(ID3D12GraphicsCommandList * cmdList)
{
	//�R�}���h���X�g���Z�b�g����
	Sprite::cmdList = cmdList;

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::EndDraw()
{
	//�R�}���h���X�g����
	Sprite::cmdList = nullptr;
}

Sprite * Sprite::Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 anchorPoint, bool isFlipX, bool isFlipY)
{
	//�����T�C�Y
	XMFLOAT2 size = { 100.0f,100.0f };

	if (texBuff[texNumber])
	{
		//�e�N�X�`���̏��擾
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();
		//�X�v���C�g�̃T�C�Y���e�N�X�`���̃T�C�Y�ɐݒ�
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	//Sprite�̃C���X�^���X�𐶐�
	Sprite* sprite = new Sprite(texNumber, position, size, 
		color, anchorPoint, isFlipX, isFlipY);

	//������
	sprite->Initialize();

	return sprite;
}

Sprite::Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorPoint, bool isFlipX, bool isFlipY)
{
	this->position = position;
	this->size = size;
	this->anchorPoint = anchorPoint;
	this->matWorld = XMMatrixIdentity();
	this->color = color;
	this->texNumber = texNumber;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;
	this->texSize = size;
}

void Sprite::Initialize()
{
	HRESULT result = S_FALSE;

#pragma region ���_���

	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexData)*vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	//���_�o�b�t�@�փf�[�^�]��
	SendVertexData();

	//���_�o�b�t�@�[�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���v�o�C�g�������߂�
	vbView.SizeInBytes = sizeof(VertexData) * 4;
	//1���_������̃o�C�g���𒲂ׂ�.
	vbView.StrideInBytes = sizeof(VertexData);

#pragma endregion

#pragma region �萔���

	//�萔�o�b�t�@�̍쐬
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\��
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	//�萔�o�b�t�@�Ƀf�[�^��]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}

#pragma endregion

}

void Sprite::SetRotation(float rotation)
{
	//�l�������ŕύX���āA���_�o�b�t�@�[�֓]������

	this->rotationZ = rotation;

	//���_�o�b�t�@�ւ̃f�[�^�]��
	SendVertexData();
}

void Sprite::SetPosition(XMFLOAT2 position)
{
	//�l�������ŕύX���āA���_�o�b�t�@�[�֓]������

	this->position = position;

	//���_�o�b�t�@�ւ̃f�[�^�]��
	SendVertexData();
}

void Sprite::SetScale(XMFLOAT2 size)
{
	//�l�������ŕύX���āA���_�o�b�t�@�[�֓]������

	this->size = size;

	//���_�o�b�t�@�ւ̃f�[�^�]��
	SendVertexData();
}

void Sprite::SetColor(XMFLOAT4 color)
{
	this->color = color;

	SendVertexData();
}

void Sprite::SetAnchorPoint(XMFLOAT2 anchorPoint)
{
	//�l�������ŕύX���āA���_�o�b�t�@�[�֓]������

	this->anchorPoint = anchorPoint;

	//���_�o�b�t�@�ւ̃f�[�^�]��
	SendVertexData();
}

void Sprite::SetFlitX(bool isFlipX)
{
	//�l�������ŕύX���āA���_�o�b�t�@�[�֓]������

	this->isFlipX = isFlipX;

	//���_�o�b�t�@�ւ̃f�[�^�]��
	SendVertexData();
}

void Sprite::SetFlitY(bool isFlipY)
{
	//�l�������ŕύX���āA���_�o�b�t�@�[�֓]������

	this->isFlipY = isFlipY;

	//���_�o�b�t�@�ւ̃f�[�^�]��
	SendVertexData();
}

void Sprite::SetTextureRect(XMFLOAT2 texBasePos, XMFLOAT2 texSize)
{
	this->texBasePos = texBasePos;
	this->texSize = texSize;

	//���_�o�b�t�@�ւ̃f�[�^�]��
	SendVertexData();
}

void Sprite::Draw()
{
	//���[���h�s��̍X�V------------------------------------------------
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);
	//------------------------------------------------------------------

	//�萔�o�b�t�@�Ƀf�[�^��]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection;
		this->constBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	//�f�B�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�萔�o�b�t�@�[�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0,
		this->constBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			this->texNumber,
			descriptorHandleIncrementSize)
	);

	//�`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SendVertexData()
{
	HRESULT result = S_FALSE;

	enum {
		LB,// 0 -- ����
		LT,// 1 -- ����
		RB,// 2 -- �E��
		RT // 3 -- �E��
	};

	//���W�̏����l
	float left =   (0.0f - anchorPoint.x) * size.x;
	float right =  (1.0f - anchorPoint.x) * size.x;
	float top =    (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	//���E����ւ�
	if (isFlipX)
	{
		left = -left;
		right = -right;
	}
	//�㉺����ւ�
	if (isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}


	//���_�f�[�^
	VertexData vertices[vertNum];

	//���W������
	vertices[LB].pos = { left,bottom, 0.0f };
	vertices[LT].pos = { left,top,    0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top,   0.0f };

	//�e�N�X�`�����擾
	if (texBuff[texNumber])
	{
		D3D12_RESOURCE_DESC resDesc = 
			texBuff[texNumber]->GetDesc();

		//uv���W�̏����l
		float tex_L = texBasePos.x / resDesc.Width;
		float tex_R = (texBasePos.x + texSize.x) / resDesc.Width;
		float tex_T = texBasePos.y / resDesc.Height;
		float tex_B = (texBasePos.y + texSize.y) / resDesc.Height;

		//uv���W������
		vertices[LB].uv = { tex_L,tex_B };
		vertices[LT].uv = { tex_L,tex_T };
		vertices[RB].uv = { tex_R,tex_B };
		vertices[RT].uv = { tex_R,tex_T };
	}

	//���_�o�b�t�@�[�փf�[�^�]��
	VertexData* vertMap = nullptr;

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

