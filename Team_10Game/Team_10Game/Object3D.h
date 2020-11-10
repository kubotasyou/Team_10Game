#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;
using namespace DirectX;

//3Dオブジェクトクラス
class Object3D
{
public://サブクラス

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;   //xyz座標
		XMFLOAT3 normal;//法線ベクトル
		XMFLOAT2 uv;	//uv座標
	};

	//定数バッファ転送データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;//色
		XMMATRIX mat;//ワールド変換行列
	};

private://定数
	static const int divison = 50;  //分割数
	static const float radius;      //半径
	static const float prizmHeight; //高さ
	static const int planeCount
		= divison * 2 + divison * 2;//面の数
	static const int vertexCount = planeCount * 3;//頂点数

public://静的メンバ関数(render)

	//静的初期化(デバイス・画面幅・画面高さ)
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//静的描画前処理(コマンドリスト)
	static void BeginDraw(ID3D12GraphicsCommandList* cmdList);

	//静的描画後処理
	static void EndDraw();

	//静的3Dオブジェクト生成
	static Object3D* Create();

	//静的視点座標の取得
	static const XMFLOAT3& GetEye()
	{
		return eye;
	}

	//静的視点座標の設定(座標)
	static void SetEye(XMFLOAT3 eyePos);

	//静的注視点座標の取得
	static const XMFLOAT3& GetTarget()
	{
		return target;
	}

	//静的注視点座標の設定(座標)
	static void SetTarget(XMFLOAT3 targetPos);

	//静的ベクトルによる移動(移動量)
	static void CameraMoveVector(XMFLOAT3 velocity);

private://静的メンバ変数(render)

	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;

	// 頂点データ配列
	static Vertex vertices[vertexCount];
	// 頂点インデックス配列
	static unsigned short indices[planeCount * 3];

private://静的メンバ関数??????

	//ディスクリプタヒープの初期化
	static void InitializeDescriptorHeap();

	//カメラ初期化(画面幅・画面高さ)
	static void InitializeCamera(int window_width, int window_height);

	//グラフィクスパイプライン生成
	static void InitializeGraphicsPipeline();

	//テクスチャ読み込み
	static void Loadtexture();

	//モデル作成
	static void CreateModel();

	//ビュー行列を更新
	static void UpdateViewmatrix();

public://メンバ関数

	void Initialize();

	void Update();

	void Draw();

	//座標取得
	const XMFLOAT3& GetPosition()
	{
		return position;
	}

	//座標の設定
	void SetPosition(XMFLOAT3 position)
	{
		this->position = position;
	}

private://メンバ変数

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//色
	XMFLOAT4 color = { 1,1,1,1 };
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	//ワールド変換行列
	XMMATRIX matWorld;
	//親オブジェクト(?)
	Object3D* parent = nullptr;

};

