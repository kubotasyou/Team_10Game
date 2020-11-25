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

	//ReadMe : �f�X�N���v�^�q�[�v�͍ŏ��Ɉ�����Ă邩�炻��������Ŏ󂯎��΂����̂ł�?

	//�f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 512;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
	//�f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Model::CreateModel(const std::string & path)
{
	HRESULT result = S_FALSE;

	//�t�@�C���X�g���[��
	std::ifstream file;

	//2-2�t�@�C���ǂݍ���(�������ς��邾���œǂݍ��߂�悤�ɂ���B)
	const string modelName = path;
	const string fileName = modelName + ".obj";
	const string directoryName = "Resources/" + modelName + "/";
	file.open(directoryName + fileName);

	//�t�@�C���ǂݍ��ݎ����s�����`�F�b�N
	if (file.fail())
	{
		//�x�����o�����
		assert(0);
	}

	vector<XMFLOAT3> positions;//���_���W
	vector<XMFLOAT3> normals;  //�@���x�N�g��
	vector<XMFLOAT2> texcoords;//uv���W

	//��s���ǂݎ��
	string line;

#pragma region �t�@�C���̓ǂݍ���

	while (std::getline(file, line))
	{
		//��s���̕�����(line)���A�X�g���[���ϊ�(���)����
		std::istringstream line_stream(line);

		//���p�X�y�[�X�ŋ�؂��āA�s�̐擪������(v�Ƃ�f)���擾
		string key;
		getline(line_stream, key, ' ');

		//�擪������'mtllib'�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			//�}�e���A���t�@�C���̓ǂݍ���
			string fileName;

			line_stream >> fileName;

			//�}�e���A���̓ǂݍ���
			LoadMaterial(directoryName, fileName);
		}

		//�擪������'v'�Ȃ璸�_���W
		if (key == "v")
		{
			//X,Y,Z���W�̓ǂݍ���
			XMFLOAT3 position{};

			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);

		}

		//�擪������'vt'�Ȃ�e�N�X�`��(uv���W)
		if (key == "vt")
		{
			//UV�����ǂݍ���
			XMFLOAT2 texcoord{};

			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			//v�������]
			texcoord.y = 1.0f - texcoord.y;
			//��y�����㉺�t�̏ꍇ�����邽�߁B

			//�e�N�X�`�����W��vector�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		//�擪������'vn'�Ȃ�@���x�N�g��
		if (key == "vn")
		{
			//X,Y,Z�����ǂݍ���
			XMFLOAT3 normal{};

			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			//�@���x�N�g����vector�f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		//�擪������'f'�Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{
			int faceIndexCount = 0;

			//���p�X�y�[�X�ŋ�؂��āA�s�̑�����ǂݍ���
			string index_string;

			while (std::getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X�P���̕������
				//�X�g���[���ɕϊ����āA��͂��₷������B
				std::istringstream index_stream(index_string);

				//���_�C���f�b�N�X�ǂݍ���
				unsigned short indexPosition, indexNormal, indexTexcoord;

				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//�X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//�X���b�V�����΂�
				index_stream >> indexNormal;

				// 2-1 �ǉ�
				//���_�f�[�^�̒ǉ�
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				//���_�C���f�b�N�X�ɒǉ�
				//.obj�t�@�C���ł́A�C���f�b�N�X��1�n�܂�Ȃ̂�
				//-1���āA0�X�^�[�g�ɍ��킹��悤�ɂ���B
				

				//��s�̓ǂݍ��ݐ���3��葽��������
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

	//�Y�ꂸ�Ƀt�@�C�������
	file.close();

	//���_�̗v�f�������Ă����B
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv)* vertices.size());
	//�C���f�b�N�X�̗v�f��������B
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)*indices.size());

#pragma region �e�o�b�t�@�̍쐬�E�]���E�r���[����

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB/*sizeof(vertices)*/),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB/*sizeof(indices)*/),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		std::copy(indices.begin(), indices.end(), indexMap);

		indexBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

}

void Model::Draw(ID3D12GraphicsCommandList * cmdList)
{
	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);

	//�`��
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Model::LoadMaterial(const std::string & directoryName, const std::string & fileName)
{
	//�t�@�C���X�g���[��
	std::ifstream file;

	material = new Material();

	//�}�e���A���t�@�C�����J��
	file.open(directoryName + fileName);

	//�t�@�C���I�[�v���̍��ۂ��`�F�b�N
	if (file.fail())
	{
		assert(0 + "�t�@�C���I�[�v���Ɏ��s");
	}

	//1�s���ǂݍ���
	string line;

	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ŁA�s�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪��Tab�����͖�������
		if (key[0] == '\t')
		{
			//�擪�̕������폜
			key.erase(key.begin());
		}

		//�擪������'newmtl'�Ȃ�}�e���A����
		if (key == "newmtl")
		{
			//�}�e���A�����ǂݍ���
			line_stream >> material->name;
		}

		//�擪������'Ka'�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}

		//�擪������'Kd'�Ȃ�f�B�t���[�Y�F
		if (key == "Kd")
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}

		//�擪������'Ks'�Ȃ�X�y�L�����[�F
		if (key == "Ks")
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}

		//�擪������'map_Kd'�Ȃ�e�N�X�`���t�@�C�����F
		if (key == "map_Kd")
		{
			//�e�N�X�`���t�@�C�����ǂݍ���
			line_stream >> material->textureFilename;

			//�e�N�X�`���ǂݍ���
			LoadTexture(directoryName, material->textureFilename);
		}
	}

	//�t�@�C�������
	file.close();
}

void Model::LoadTexture(const std::string & directoryName, const std::string & fileName)
{
	HRESULT result = S_FALSE;

	//�t�@�C���p�X������
	string filepath = directoryName + fileName;

	//���j�R�[�h������ɕϊ�����
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(
		CP_ACP,
		0,
		filepath.c_str(),
		-1,
		wfilepath,
		_countof(wfilepath)
	);

	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0); //���f�[�^���o

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels)
	);

	//�e�N�X�`���p�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, //�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texbuff->WriteToSubresource(
		0,
		nullptr,                          //�S�̈�փR�s�[
		img->pixels,                      //���f�[�^�A�h���X
		static_cast<UINT>(img->rowPitch), //1���C���T�C�Y
		static_cast<UINT>(img->slicePitch)//1���T�C�Y
	);

	//ReadMe : �V�F�[�_�[���\�[�X�r���[���Ċe�I�u�W�F�N�g�Ɉ�K�v�Ȃ񂾂���?

	//�V�F�[�_���\�[�X�r���[�쐬
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff.Get(),//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,                                  //�e�N�X�`���ݒ���
		cpuDescHandleSRV
	);

}
