#pragma once
#include "Model.h"

class GameObject
{
private:// Microsoft::WRL::を省略
	
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://サブクラス

	//定数バッファ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// ３Ｄ変換行列
		XMFLOAT4 color;
	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; //アンビエント
		float pad1;		  //パティング
		XMFLOAT3 diffuse; //ディフューズ
		float pad2;		  //パティング
		XMFLOAT3 specular;//スペキュラー
		float alpha;      //パティング
	};

public://一回しか読んじゃいけないやつら

	//ReadMe : staticが気になるから使わない方法を考える

	//一度だけの初期化(デバイス・画面幅・画面高さ)
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//描画前処理(コマンドリスト)
	static void BeginDraw(ID3D12GraphicsCommandList* cmdList);

	//描画後処理
	static void EndDraw();

	//ReadMe : 3Dオブジェクト生成はstaticを使わないで作る
	static GameObject* Create();

	//視点座標設定
	static void SetEye(XMFLOAT3 eye);

	//注視点座標設定
	static void SetTarget(XMFLOAT3 target);

	//視点座標取得
	static const XMFLOAT3& GetEye()
	{
		return eye;
	}

	//注視点座標取得
	static const XMFLOAT3& GetTarget()
	{
		return target;
	}

	//ベクトルによるカメラの移動
	static void CameraMoveVector(XMFLOAT3 velocity);

	//カメラの移動(ターゲット変更なし)
	static void CameraRotVector(XMFLOAT3 rotation);

private://一回しか読んじゃいけないやつらpart2

	//デスクリプタヒープの初期化
	static void InitializeDescriptorHeap();

	//カメラの初期化(画面幅・画面高さ)
	static void InitializeCamera(int winodw_width, int window_height);

	//グラフィクスパイプラインの初期化
	static void InitializeGraphicsPipeline();

	//ビュー行列を更新
	static void UpdateViewMatrix();

public://関数

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	//モデルのセット
	void SetModel(Model* _model);

	//座標の設定
	void SetPosition(XMFLOAT3 position)
	{
		this->position = position;
	}

	//色の設定
	void SetColor(XMFLOAT4 color)
	{
		this->color = color;
	}

	//大きさの設定
	void SetScale(XMFLOAT3 scale)
	{
		this->scale = scale;
	}

	//座標の取得
	const XMFLOAT3& GetPosition()
	{
		return position;
	}

	//半径の設定
	void SetRadius(float radius)
	{
		this->radius = radius;
	}

	//半径取得
	const float& GetRadius()
	{
		return radius;
	}

private://一度しか呼ばないメンバ変数

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

private://メンバ変数

	Model* model = nullptr;

	ComPtr<ID3D12Resource> constBuffB0;//定数バッファB0
	ComPtr<ID3D12Resource> constBuffB1;//定数バッファB1
		
	XMFLOAT4 color = { 1,1,1,1 }; //色
	XMFLOAT3 scale = { 1,1,1 };   //ローカルスケール	
	XMFLOAT3 rotation = { 0,0,0 };//X,Y,Z軸回りのローカル回転角
	XMFLOAT3 position = { 0,0,0 };//ローカル座標
	XMMATRIX matWorld;            //ローカルワールド変換行列

	//半径
	float radius = 1.0f;

};

