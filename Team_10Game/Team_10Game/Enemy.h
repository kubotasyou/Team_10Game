#pragma once

#include "GameObject.h"
#include "Model.h"
#include "CollisionPrimitive.h"
#include "CountDownTimer.h"

//�G�N���X�F�����痬��Ă���
//memo : ���X�|�[���̏������܂��ł��ĂȂ�
class Enemy
{
private:
	// DirectX::���ȗ�
	using float3 = DirectX::XMFLOAT3;

public:
	Enemy(Model* model);
	~Enemy();

	void Initialize();

	void Update();

	void Draw();

	//�G�̓����蔻���n���B
	Sphere GetSphere() { return sphere; }
	//���S�����n���B
	bool GetDeadFlag() { return isDead; }
	//���S�����ς���B
	void ChangeDeadFlag(bool value);

private:

	//�G�̈ړ�
	void Move();

private:
	GameObject* enemy = nullptr;
	Model* enemyModel = nullptr;

	float3 position;
	float3 velocity;
	float speed = 0.05f;

	float3 playerPosition;//�v���C���[�̈ʒu(����͌Œ�B)
	float3 dist;//�v���C���[�ƓG�Ƃ̋���
	float distance;//�v���C���[�ƓG�̋����̕�����

	//�����蔻��F��
	Sphere sphere;

	bool isDead = false;//���S����B
};

