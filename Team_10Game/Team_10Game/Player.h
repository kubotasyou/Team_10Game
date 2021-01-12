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
	// DirectX::���ȗ�
	using float3 = DirectX::XMFLOAT3;

public:
	Player(Input * input, Model* model);
	~Player();

	void Initialize();

	void Update();

	void Draw();

	//�e����
	void Shot();

	//���݈ʒu���擾
	float3 GetPosition() { return position; }
	//���@�̓����蔻����擾
	Sphere GetSphere() { return sphere; }

	//�e�̃��X�g���擾������
	std::vector<Bullet*> GetBulletList() { return bulletList; }

	//�ǉ�
	void ChangeDamageFlag(bool flag);
	bool GetDamageFlag() { return DamageFlag; }
	///

private:

	//���@�̈ړ�
	void Move();

private:
	Input* input = nullptr;
	GameObject* player = nullptr;
	Model* sphereModel = nullptr;

	float3 position;
	float3 velocity;

	float speed = 0.1f;
	float hp = 3;

	bool DamageFlag =false;

	//�e�̃��X�g
	std::vector<Bullet*> bulletList;

	//���@�̓����蔻��
	Sphere sphere;
};

