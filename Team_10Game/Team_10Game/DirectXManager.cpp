#include "DirectXManager.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

//DirectXの初期化+画面のクリア

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
	//バックバッファの番号を取得(表と裏しかないので0or1)
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//リソースバリアの変更(表示状態→描画状態)
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//[レンダーターゲット用]ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			bbIndex,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//[深度ステンシルビュー用]ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//レンダーターゲットビューをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//画面のクリア
	ClearRenderTarget();
	//深度バッファのクリア
	ClearDepthBuffer();
	//ビューポート・シザー矩形の設定
	Initialize_Screen();
}

void DirectXManager::EndDraw()
{
	//バックバッファの番号を取得(表と裏しかないので0or1)
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//リソースバリアの変更(描画状態→表示状態)
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT));

	//命令のクローズ
	cmdList->Close();

	//コマンドリストの実行
	ID3D12CommandList* _cmdLists[] = { cmdList.Get() };//コマンドリストの配列
	cmdQueue->ExecuteCommandLists(1, _cmdLists);

	//コマンドリストの実行が終了するのを待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);

	//フェンスの呼び出し
	if (fence->GetCompletedValue() != fenceVal)
	{
		//イベントハンドルの取得
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);

		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//コマンドのクリア
	cmdAllocator->Reset();
	//再びコマンドリストをため込む準備
	cmdList->Reset(cmdAllocator.Get(), nullptr);

	//バックバッファのフリップ
	swapchain->Present(1, 0);
}

void DirectXManager::ClearRenderTarget()
{
	//バックバッファの番号を取得(表と裏しかないので0or1)
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//[レンダーターゲット用]ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			bbIndex,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// 全画面クリア        Red   Green Blue  Alpha
	float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f };

	//画面のクリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DirectXManager::ClearDepthBuffer()
{
	//[深度ステンシルビュー用]ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DirectXManager::InitializeDevice()
{
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//デバッグレイヤーをオンに	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif


	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	//アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter>> adapters;

	//アダプターオブジェクトを格納する変数
	ComPtr<IDXGIAdapter> tmpAdapter;

	for (int i = 0;
		dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		//利用可能なアダプターを配列に格納する
		adapters.push_back(tmpAdapter);
	}
	for (int i = 0; i < adapters.size(); i++)
	{
		//アダプターの設定構造体
		DXGI_ADAPTER_DESC adDesc{};//AdapterDesc

		//アダプターの情報を取得
		adapters[i]->GetDesc(&adDesc);

		//アダプター名
		std::wstring strDesc = adDesc.Description;

		//探したいアダプターの名前を確認
		if (strDesc.find(L"NAVIDATA") != std::string::npos)
		{
			//見つかったら_tmpAdapterに格納
			tmpAdapter = adapters[i];
			break;
		}
	}

	//使用できるレベルを一つ一つ調べる
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	//フューチャーレベルの初期化
	D3D_FEATURE_LEVEL _featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));

		//デバイスを生成できたらループを抜ける
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

	//スワップチェーンの設定構造体
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};

	swapchainDesc.Width = WinApp::window_width;
	swapchainDesc.Height = WinApp::window_height;

	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       //色情報の書式
	swapchainDesc.SampleDesc.Count = 1;                      //サンプリングをしない→1
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;      //バックバッファを使用
	swapchainDesc.BufferCount = 2;                           //バックバッファをしようするため2
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄する
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//IDXGISwapChain1のComPtrを用意
	ComPtr<IDXGISwapChain1> swapChain1;
	//設定の反映

	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		winApp->GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapChain1
	);

	//生成したswapChain1をswapChain4に変換する
	swapChain1.As(&swapchain);
}

void DirectXManager::InitializeCommand()
{
	HRESULT result = S_FALSE;

	//コマンドアロケータの生成
	result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));

	//コマンドリストの生成
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	//コマンドキューの生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
}

void DirectXManager::CreateRenderTarget()
{
	HRESULT result = S_FALSE;

	//スワップチェーンの設定を見る。
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);

	//ディスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	//レンダーターゲットビューを指定
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//バックバッファを使うため、表と裏の二つ
	heapDesc.NumDescriptors = 2;
	//設定の反映
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	//------------------------------------------------------------------------

	//バックバッファの数を設定
	backBuffers.resize(swcDesc.BufferCount);
	//表と裏の二つ分について
	for (int i = 0; i < backBuffers.size(); i++)
	{
		//スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		//ヘルパー構造体を使い、ヒープ配列にアクセスしやすくする
		//（先頭ハンドル・ディスクリプタの番号・一つ分の大きさの指定）
		CD3DX12_CPU_DESCRIPTOR_HANDLE _handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i,
				device->GetDescriptorHandleIncrementSize(heapDesc.Type));

		//レンダーターゲットビューの生成
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

	//ヘルパー構造体を使った深度バッファの設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//ヘルパー構造体を使った深度バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));

	//--------------------------------------------------------

	//深度ビューの作成
	//深度ビュー用のディスクリプタヒープを作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};

	//深度ビューは一つ
	dsvHeapDesc.NumDescriptors = 1;
	//デプスステンシルビュー???
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	//リソース生成
	result = device->CreateDescriptorHeap(
		&dsvHeapDesc,
		IID_PPV_ARGS(&dsvHeap)
	);

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	//深度値フォーマット
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//生成
	device->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void DirectXManager::CreateFence()
{
	HRESULT result = S_FALSE;

	//フェンスの生成
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&fence));
}

void DirectXManager::Initialize_Screen()
{
	//ビューポートの設定
	cmdList->RSSetViewports(1,
		&CD3DX12_VIEWPORT(
			0.0f, 0.0f,
			WinApp::window_width, WinApp::window_height
		));

	//シザー矩形の設定
	cmdList->RSSetScissorRects(1,
		&CD3DX12_RECT(
			0,    0,
			WinApp::window_width, WinApp::window_height
		));
}
