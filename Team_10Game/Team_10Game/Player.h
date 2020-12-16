#pragma once

#include "DirectXManager.h"
#include "Input.h"
#include "GameObject.h"
#include "Model.h"
#include "Sound.h"

#include "CountDownTimer.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class Player
{
public:
	Player(DirectXManager * dxManager, Input * input);
	~Player();

	void Initialize();

	void Update();

	void Draw();

	XMFLOAT3 GetPosition() { return position; }

	XMFLOAT3 GetColliderPos() { return collider->GetPosition(); }

	bool GetisGround() { return isGround; }

	float GetColliderRadius() { return colliderRadius; }

	void SetisGround(bool isGround)
	{
		this->isGround = isGround;
	}

private:

	void Move();

	void Jump();

private:
	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;
	Sound* sound = nullptr;
	GameObject* player = nullptr;
	Model* sphereModel = nullptr;

	XMFLOAT3 position;
	XMFLOAT3 velocity;

	GameObject* collider = nullptr;

	float speed = 0.1f;

	float colliderRadius = 0.1f;
	bool isGround = false;

	bool isJumpFlag = false;
	float jumpPower = 3.0f;

	float prePos;

	CountDownTimer* timer = nullptr;

	//ジャンプさせたい高さを設定して、それに到達するまでyの値を増やし続ける。
	//増やし続けている間は、重力を使わない
};

