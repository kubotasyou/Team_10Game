#pragma once

#include "DirectXManager.h"
#include "Input.h"
#include "GameObject.h"
#include "Model.h"
#include "Bullet.h"

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

private:

	void Move();

private:
	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;
	GameObject* player = nullptr;
	Model* sphereModel = nullptr;

	//バレットのリスト
	std::vector<Bullet*> bl;


	XMFLOAT3 position;
	XMFLOAT3 velocity;

	float speed = 0.1f;
};

