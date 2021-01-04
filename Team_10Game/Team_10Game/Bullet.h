#pragma once

#include "DirectXManager.h"
#include "GameObject.h"
#include "Model.h"

//�e�N���X

class Bullet
{
public:
	Bullet(DirectXManager* dxManager, XMFLOAT3 position);
	~Bullet();

	void Update();

	void Draw();

	//�g���Ă��邩�ǂ�����؂�ւ���
	void ChangeUsed(bool flag);

	bool GetUsedFlag() { return isUsed; }

private:
	DirectXManager* dxManager = nullptr;
	GameObject* bullet = nullptr;
	Model* sphereModel = nullptr;

	XMFLOAT3 position;
	XMFLOAT3 velocity;

	float speed = 0.1f;
	float radius = 1.0f;

	//�g���Ă��邩�ǂ����̃t���O
	bool isUsed;
};

