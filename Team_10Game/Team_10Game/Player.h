#pragma once

#include "DirectXManager.h"
#include "Input.h"
#include "GameObject.h"
#include "Model.h"
#include "CollisionPrimitive.h"
#include "Bullet.h"

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

	//弾発射
	void Shot();

	//現在位置を取得
	float3 GetPosition() { return position; }
	//自機の当たり判定を取得
	Sphere GetSphere() { return sphere; }

	//弾のリストを取得したい
	std::vector<Bullet*> GetBulletList() { return bulletList; }

private:

	//自機の移動
	void Move();

private:
	Input* input = nullptr;
	GameObject* player = nullptr;
	Model* sphereModel = nullptr;

	float3 position;
	float3 velocity;

	float speed = 0.1f;
	float hp = 3;

	//弾のリスト
	std::vector<Bullet*> bulletList;

	//自機の当たり判定
	Sphere sphere;
};

