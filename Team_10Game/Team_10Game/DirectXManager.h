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
	//コンスト
	DirectXManager();

	//デスト
	~DirectXManager();

	//初期化
	void Initialize(WinApp* win);

	//描画前処理
	void BeginDraw();

	//描画後処理
	void EndDraw();

	//レンダーターゲットのクリア
	void ClearRenderTarget();

	//深度バッファのクリア
	void ClearDepthBuffer();

	//デバイスの取得
	ID3D12Device* GetDevice()
	{
		return device.Get();
	}

	//コマンドリストの取得
	ID3D12GraphicsCommandList* GetcmdList()
	{
		return cmdList.Get();
	}

private://メンバ変数

	//HRESULT result;

	// Direct3D関連----------------------------------
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

	//ウィンドウアプリ
	WinApp* winApp;

private://メンバ関数

	//デバイスの初期化
	void InitializeDevice();

	//スワップチェーンの生成
	void CreateSwapChain();

	//コマンド関連初期化
	void InitializeCommand();

	//レンダーターゲットの生成
	void CreateRenderTarget();

	//深度バッファ生成
	void CreateDepthBuffer();

	//フェンスの生成
	void CreateFence();

	//ビューポート・シザー矩形の設定
	void Initialize_Screen();

};

