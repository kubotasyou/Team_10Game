#pragma once

#include<Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>

#include <cstdlib>

#include "WinApp.h"

using namespace Microsoft::WRL;

class DirectXManager
{
public:
	//�R���X�g
	DirectXManager();

	//�f�X�g
	~DirectXManager();

	//������
	void Initialize(WinApp* win);

	//�`��O����
	void BeginDraw();

	//�`��㏈��
	void EndDraw();

	//�����_�[�^�[�Q�b�g�̃N���A
	void ClearRenderTarget();

	//�[�x�o�b�t�@�̃N���A
	void ClearDepthBuffer();

	//�f�o�C�X�̎擾
	ID3D12Device* GetDevice()
	{
		return device.Get();
	}

	//�R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* GetcmdList()
	{
		return cmdList.Get();
	}

private://�����o�ϐ�

	//HRESULT result;

	// Direct3D�֘A----------------------------------
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	//------------------------------------------------

	//�E�B���h�E�A�v��
	WinApp* winApp;

private://�����o�֐�

	//�f�o�C�X�̏�����
	void InitializeDevice();

	//�X���b�v�`�F�[���̐���
	void CreateSwapChain();

	//�R�}���h�֘A������
	void InitializeCommand();

	//�����_�[�^�[�Q�b�g�̐���
	void CreateRenderTarget();

	//�[�x�o�b�t�@����
	void CreateDepthBuffer();

	//�t�F���X�̐���
	void CreateFence();

	//�r���[�|�[�g�E�V�U�[��`�̐ݒ�
	void Initialize_Screen();

};

