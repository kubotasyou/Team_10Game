#pragma once

#include "DirectXManager.h"
#include "Input.h"
#include "GameObject.h"
#include "Model.h"
#include "CollisionPrimitive.h"
#include "Bullet.h"

#include "CountDownTimer.h"

class Player
{
private:
	// DirectX::を省略
	using float3 = DirectX::XMFLOAT3;

public:
	Player(Input * input, Model* model);
	~Player();

	void Initialize();

	void Update();

	void Draw();

	//ダメージ処理
	void Damage(int damage);

	//現在位置を取得
	float3 GetPosition() { return position; }
	//自機の当たり判定を取得
	Sphere GetSphere() { return sphere; }
	//現在の体力取得
	int GetHp() { return hp; }

	//弾のリストを取得したい
	std::vector<Bullet*> GetBulletList() { return bulletList; }

private:

	//自機の移動
	void Move();

	//カメラの移動
	void CameraMove();

	//弾発射
	void Shot();

	//ダメージの点滅
	void Blinking();

	//範囲の制限(クランプしたい値・最小値・最大値)
	float Clamp(float value, float min, float max);

private:
	Input* input = nullptr;
	GameObject* player = nullptr;
	GameObject* camera = nullptr;//カメラ操作用
	Model* sphereModel = nullptr;

	float3 position;//自機の位置
	float3 velocity;//自機の移動量

	float3 cameraPosition;//カメラの位置
	float3 cameraVelocity;//カメラの移動量

	float speed = 0.2f;//移動速度
	int hp = 3;        //体力

	//弾のリスト
	std::vector<Bullet*> bulletList;

	//自機の当たり判定
	Sphere sphere;

	//ダメージフラグ
	bool damageFlag = false;

	//ダメージ用タイマー
	CountDownTimer* downTimer;

	//無敵時間
	float noDamageTime = 3.0f;

	//点滅用カウント
	int count = 0;
};

