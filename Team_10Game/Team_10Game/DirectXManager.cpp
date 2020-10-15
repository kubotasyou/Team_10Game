#include "DirectXManager.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

//DirectX�̏�����+��ʂ̃N���A

DirectXManager::DirectXManager()
{
}

DirectXManager::~DirectXManager()
{
}

void DirectXManager::Initialize(WinApp* win)
{
	winApp = win;

	InitializeDevice();
	InitializeCommand();
	CreateSwapChain();
	CreateRenderTarget();
	CreateDepthBuffer();
	CreateFence();
}

void DirectXManager::BeginDraw()
{
	//�o�b�N�o�b�t�@�̔ԍ����擾(�\�Ɨ������Ȃ��̂�0or1)
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//���\�[�X�o���A�̕ύX(�\����ԁ��`����)
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//[�����_�[�^�[�Q�b�g�p]�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			bbIndex,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//[�[�x�X�e���V���r���[�p]�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//�����_�[�^�[�Q�b�g�r���[���Z�b�g
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//��ʂ̃N���A
	ClearRenderTarget();
	//�[�x�o�b�t�@�̃N���A
	ClearDepthBuffer();
	//�r���[�|�[�g�E�V�U�[��`�̐ݒ�
	Initialize_Screen();
}

void DirectXManager::EndDraw()
{
	//�o�b�N�o�b�t�@�̔ԍ����擾(�\�Ɨ������Ȃ��̂�0or1)
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//���\�[�X�o���A�̕ύX(�`���ԁ��\�����)
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT));

	//���߂̃N���[�Y
	cmdList->Close();

	//�R�}���h���X�g�̎��s
	ID3D12CommandList* _cmdLists[] = { cmdList.Get() };//�R�}���h���X�g�̔z��
	cmdQueue->ExecuteCommandLists(1, _cmdLists);

	//�R�}���h���X�g�̎��s���I������̂�҂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);

	//�t�F���X�̌Ăяo��
	if (fence->GetCompletedValue() != fenceVal)
	{
		//�C�x���g�n���h���̎擾
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);

		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//�R�}���h�̃N���A
	cmdAllocator->Reset();
	//�ĂуR�}���h���X�g�����ߍ��ޏ���
	cmdList->Reset(cmdAllocator.Get(), nullptr);

	//�o�b�N�o�b�t�@�̃t���b�v
	swapchain->Present(1, 0);
}

void DirectXManager::ClearRenderTarget()
{
	//�o�b�N�o�b�t�@�̔ԍ����擾(�\�Ɨ������Ȃ��̂�0or1)
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//[�����_�[�^�[�Q�b�g�p]�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			bbIndex,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// �S��ʃN���A        Red   Green Blue  Alpha
	float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f };

	//��ʂ̃N���A
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DirectXManager::ClearDepthBuffer()
{
	//[�[�x�X�e���V���r���[�p]�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DirectXManager::InitializeDevice()
{
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//�f�o�b�O���C���[���I����	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif


	//DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	//�A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter>> adapters;

	//�A�_�v�^�[�I�u�W�F�N�g���i�[����ϐ�
	ComPtr<IDXGIAdapter> tmpAdapter;

	for (int i = 0;
		dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		//���p�\�ȃA�_�v�^�[��z��Ɋi�[����
		adapters.push_back(tmpAdapter);
	}
	for (int i = 0; i < adapters.size(); i++)
	{
		//�A�_�v�^�[�̐ݒ�\����
		DXGI_ADAPTER_DESC adDesc{};//AdapterDesc

		//�A�_�v�^�[�̏����擾
		adapters[i]->GetDesc(&adDesc);

		//�A�_�v�^�[��
		std::wstring strDesc = adDesc.Description;

		//�T�������A�_�v�^�[�̖��O���m�F
		if (strDesc.find(L"NAVIDATA") != std::string::npos)
		{
			//����������_tmpAdapter�Ɋi�[
			tmpAdapter = adapters[i];
			break;
		}
	}

	//�g�p�ł��郌�x���������ׂ�
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	//�t���[�`���[���x���̏�����
	D3D_FEATURE_LEVEL _featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));

		//�f�o�C�X�𐶐��ł����烋�[�v�𔲂���
		if (result == S_OK)
		{
			_featureLevel = levels[i];
			break;
		}
	}
}

void DirectXManager::CreateSwapChain()
{
	HRESULT result = S_FALSE;

	//�X���b�v�`�F�[���̐ݒ�\����
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};

	swapchainDesc.Width = WinApp::window_width;
	swapchainDesc.Height = WinApp::window_height;

	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       //�F���̏���
	swapchainDesc.SampleDesc.Count = 1;                      //�T���v�����O�����Ȃ���1
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;      //�o�b�N�o�b�t�@���g�p
	swapchainDesc.BufferCount = 2;                           //�o�b�N�o�b�t�@�����悤���邽��2
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͔j������
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//IDXGISwapChain1��ComPtr��p��
	ComPtr<IDXGISwapChain1> swapChain1;
	//�ݒ�̔��f

	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		winApp->GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapChain1
	);

	//��������swapChain1��swapChain4�ɕϊ�����
	swapChain1.As(&swapchain);
}

void DirectXManager::InitializeCommand()
{
	HRESULT result = S_FALSE;

	//�R�}���h�A���P�[�^�̐���
	result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));

	//�R�}���h���X�g�̐���
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	//�R�}���h�L���[�̐���
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
}

void DirectXManager::CreateRenderTarget()
{
	HRESULT result = S_FALSE;

	//�X���b�v�`�F�[���̐ݒ������B
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);

	//�f�B�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	//�����_�[�^�[�Q�b�g�r���[���w��
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//�o�b�N�o�b�t�@���g�����߁A�\�Ɨ��̓��
	heapDesc.NumDescriptors = 2;
	//�ݒ�̔��f
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	//------------------------------------------------------------------------

	//�o�b�N�o�b�t�@�̐���ݒ�
	backBuffers.resize(swcDesc.BufferCount);
	//�\�Ɨ��̓���ɂ���
	for (int i = 0; i < backBuffers.size(); i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		//�w���p�[�\���̂��g���A�q�[�v�z��ɃA�N�Z�X���₷������
		//�i�擪�n���h���E�f�B�X�N���v�^�̔ԍ��E����̑傫���̎w��j
		CD3DX12_CPU_DESCRIPTOR_HANDLE _handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i,
				device->GetDescriptorHandleIncrementSize(heapDesc.Type));

		//�����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			_handle
		);
	}
}

void DirectXManager::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	//�w���p�[�\���̂��g�����[�x�o�b�t�@�̐ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//�w���p�[�\���̂��g�����[�x�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));

	//--------------------------------------------------------

	//�[�x�r���[�̍쐬
	//�[�x�r���[�p�̃f�B�X�N���v�^�q�[�v���쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};

	//�[�x�r���[�͈��
	dsvHeapDesc.NumDescriptors = 1;
	//�f�v�X�X�e���V���r���[???
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	//���\�[�X����
	result = device->CreateDescriptorHeap(
		&dsvHeapDesc,
		IID_PPV_ARGS(&dsvHeap)
	);

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	//�[�x�l�t�H�[�}�b�g
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//����
	device->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void DirectXManager::CreateFence()
{
	HRESULT result = S_FALSE;

	//�t�F���X�̐���
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&fence));
}

void DirectXManager::Initialize_Screen()
{
	//�r���[�|�[�g�̐ݒ�
	cmdList->RSSetViewports(1,
		&CD3DX12_VIEWPORT(
			0.0f, 0.0f,
			WinApp::window_width, WinApp::window_height
		));

	//�V�U�[��`�̐ݒ�
	cmdList->RSSetScissorRects(1,
		&CD3DX12_RECT(
			0,    0,
			WinApp::window_width, WinApp::window_height
		));
}
