#pragma once

#include "GameObject.h"
#include "Model.h"
#include "CollisionPrimitive.h"

//弾クラス
class Bullet
{
private:
	// DirectX::を省略
	using float3 = DirectX::XMFLOAT3;

public:
	Bullet(float3 position,Model* model,float3 pointerposition);
	~Bullet();

	void Update();

	void Draw();

	float3 Position() { return position; }

	//使っているかどうかを切り替える
	void ChangeUseFlag(bool flag);
	//死んでいるかどうかを切り替える
	void ChangeDeadFlag(bool flag);

	//使っているかどうかを切り替える
	bool GetUsedFlag() { return isUsed; }
	//死んでいるかどうかを切り替える
	bool GetDeadFlag() { return isDeadFlag; }

	//当たり判定を取得
	Sphere GetSphere() { return sphere; }

private:

	//弾の移動
	void Move();

private:
	GameObject* bullet = nullptr;
	Model* sphereModel = nullptr;

	float3 position;//位置
	float3 velocity;//移動量
	float3 pointerPosition;//照準の位置
	float3 dist;//照準と弾の距離
	float distance;//照準と敵の距離

	float speed = 0.1f;//移動速度

	//使っているかどうかのフラグ
    bool isUsed;
	bool isDeadFlag;

	//当たり判定：球
	Sphere sphere;
};

