#pragma once

#include "GameObject.h"
#include "Model.h"
#include "CollisionPrimitive.h"
#include "CountDownTimer.h"
#include "ParticleManager.h"

//�G�N���X�F�����痬��Ă���
class Enemy
{
private:
	// DirectX::���ȗ�
	using float3 = DirectX::XMFLOAT3;

public:
	Enemy(Model* model);
	~Enemy();

	void Initialize(float3 playerPosition);

	void Update(float3 playerPosition);

	void Draw();

	//�G�̓����蔻���n���B
	Sphere GetSphere() { return sphere; }
	//���S�����n���B
	bool GetDeadFlag() { return isDead; }
	//���S�����ς���B
	void ChangeDeadFlag(bool value);

	float3 GetPosition() { return position; }

private:

	//�G�̈ړ�
	void Move();

private:
	GameObject* enemy = nullptr;
	Model* enemyModel = nullptr;

	float3 position;
	float3 velocity;
	float speed = 0.2f;

	float3 playerPosition;//�v���C���[�̈ʒu
	float3 dist;//�v���C���[�ƓG�Ƃ̋���
	float distance;//�v���C���[�ƓG�̋����̕�����

	//�����蔻��F��
	Sphere sphere;

	bool isDead = false;//���S����

	bool distFlag = false;//�v���C���[�ƈ�苗���ȓ��ɂȂ�����
};

