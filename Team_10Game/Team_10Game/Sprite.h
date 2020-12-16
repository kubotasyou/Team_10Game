#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class Sprite
{
public://シェーダに送るデータ

	//頂点データ
	struct VertexData
	{
		XMFLOAT3 pos;//xyz座標
		XMFLOAT2 uv; //uv座標
	};

	//定数データ
	struct ConstBufferData
	{
		XMFLOAT4 color;//色
		XMMATRIX mat;  //3D変換行列
	};

public://static 共通のメンバ関数

	//共通初期化(デバイス・画面幅・画面高さ)
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//テクスチャ読み込み(テクスチャ番号・画像ファイル名)
	static void LoadTexture(UINT texNumber, const wchar_t* fileName);

	//描画開始処理(描画コマンドリスト)
	static void BeginDraw(ID3D12GraphicsCommandList* cmdList);

	//描画終了処理
	static void EndDraw();

	//スプライト生成(生成されたスプライトを返す)
	//(テクスチャ番号・座標・色・アンカーポイント・左右反転・上下反転)
	static Sprite* Create(UINT texNumber, XMFLOAT2 position,
		XMFLOAT4 color = { 1,1,1,1 }, XMFLOAT2 anchorPoint = {0.0f,0.0f},
		bool isFlipX = false, bool isFlipY = false);

private://static 変数

	//テクスチャの最大枚数(SRVの数?)
	static const int srvCount = 512;

	//頂点数(四角形)
	static const int vertNum = 4;

	//デバイス
	static ID3D12Device* device;

	//ディスクリプタサイズ
	static UINT descriptorHandleIncrementSize;

	//描画コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;

	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;

	//パイプラインステート
	static ComPtr<ID3D12PipelineState> pipelineState;

	//射影変換行列
	static XMMATRIX matProjection;

	//ディスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;

	//テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff[srvCount];

public://メンバ関数

	//コンスト
	//(テクスチャ番号・座標・サイズ・色・アンカーポイント
	// 左右反転・上下反転)
	Sprite(UINT texNumber, XMFLOAT2 position,
		XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorPoint,
		bool isFlipX, bool isFlipY);

	//初期化
	void Initialize();

	//回転角設定(角度)
	void SetRotation(float rotation);

	//座標設定(位置)
	void SetPosition(XMFLOAT2 position);

	//サイズ設定(大きさ)
	void SetScale(XMFLOAT2 size);

	//色の変更(色)
	void SetColor(XMFLOAT4 color);

	//アンカーポイントの設定(中心点)
	void SetAnchorPoint(XMFLOAT2 anchorPoint);

	//左右反転の設定(左右反転するかどうか)
	void SetFlitX(bool isFlipX);

	//上下反転の設定(上下反転するかどうか)
	void SetFlitY(bool isFlipY);

	//テクスチャ範囲指定(テクスチャの左上座標・テクスチャサイズ)
	void SetTextureRect(XMFLOAT2 texBasePos, XMFLOAT2 texSize);

	//描画
	void Draw();

private://メンバ変数

	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//頂点バッファービュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//テクスチャ番号
	UINT texNumber = 0;
	//Z軸周りの回転角
	float rotationZ = 0.0f;
	//座標
	XMFLOAT2 position{};
	//スプライトの幅、高さ(初期値)
	XMFLOAT2 size = { 100.0f,100.0f };
	//アンカーポイント(中心点)
	XMFLOAT2 anchorPoint = { 0,0 };
	//ワールド行列
	XMMATRIX matWorld{};
	//色
	XMFLOAT4 color = { 1,1,1,1 };
	//左右反転
	bool isFlipX = false;
	//上下反転
	bool isFlipY = false;

	//テクスチャ始点
	XMFLOAT2 texBasePos = { 0,0 };
	//テクスチャの幅、高さ
	XMFLOAT2 texSize = { 100.0f,100.0f };

private://メンバ関数

	//頂点データ転送
	void SendVertexData();

};
