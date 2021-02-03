#pragma once

#include "GameObject.h"
#include "Model.h"
#include "CollisionPrimitive.h"

//�e�N���X
class Bullet
{
private:
	// DirectX::���ȗ�
	using float3 = DirectX::XMFLOAT3;

public:
	Bullet(float3 position,Model* model,float3 pointerposition);
	~Bullet();

	void Update();

	void Draw();

	float3 Position() { return position; }

	//�g���Ă��邩�ǂ�����؂�ւ���
	void ChangeUseFlag(bool flag);
	//����ł��邩�ǂ�����؂�ւ���
	void ChangeDeadFlag(bool flag);

	//�g���Ă��邩�ǂ�����؂�ւ���
	bool GetUsedFlag() { return isUsed; }
	//����ł��邩�ǂ�����؂�ւ���
	bool GetDeadFlag() { return isDeadFlag; }

	//�����蔻����擾
	Sphere GetSphere() { return sphere; }

private:

	//�e�̈ړ�
	void Move();

private:
	GameObject* bullet = nullptr;
	Model* sphereModel = nullptr;

	float3 position;//�ʒu
	float3 velocity;//�ړ���
	float3 pointerPosition;//�Ə��̈ʒu
	float3 dist;//�Ə��ƒe�̋���
	float distance;//�Ə��ƓG�̋���

	float speed = 0.1f;//�ړ����x

	//�g���Ă��邩�ǂ����̃t���O
    bool isUsed;
	bool isDeadFlag;

	//�����蔻��F��
	Sphere sphere;
};

