#pragma once

#include "GameObject.h"
#include "Model.h"
#include "CollisionPrimitive.h"
#include "CountDownTimer.h"

//敵クラス：奥から流れてくる
//memo : リスポーンの処理がまだできてない
class Enemy
{
private:
	// DirectX::を省略
	using float3 = DirectX::XMFLOAT3;

public:
	Enemy(Model* model);
	~Enemy();

	void Initialize();

	void Update();

	void Draw();

	//敵の当たり判定を渡す。
	Sphere GetSphere() { return sphere; }
	//死亡判定を渡す。
	bool GetDeadFlag() { return isDead; }
	//死亡判定を変える。
	void ChangeDeadFlag(bool value);

private:

	//敵の移動
	void Move();

private:
	GameObject* enemy = nullptr;
	Model* enemyModel = nullptr;

	float3 position;
	float3 velocity;
	float speed = 0.05f;

	float3 playerPosition;//プレイヤーの位置(今回は固定。)
	float3 dist;//プレイヤーと敵との距離
	float distance;//プレイヤーと敵の距離の平方根

	//当たり判定：球
	Sphere sphere;

	bool isDead = false;//死亡判定。
};

