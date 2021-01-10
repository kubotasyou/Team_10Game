#pragma once

#include "GameObject.h"
#include "Model.h"

//�e�N���X

class Bullet
{
public:
	Bullet(XMFLOAT3 position,Model* model);
	~Bullet();

	void Update();

	void Draw();

	XMFLOAT3 Position() { return position; }

	//�g���Ă��邩�ǂ�����؂�ւ���
	void ChangeUsed(bool flag);
	void ChangeDelete(bool flag);

	bool GetUsedFlag() { return isUsed; }
	bool GetDeleteFlag() { return isDelete; }
private:
	GameObject* bullet = nullptr;
	Model* sphereModel = nullptr;


	XMFLOAT3 position;
	XMFLOAT3 velocity;
	XMFLOAT3 playerposition;

	float speed = 0.1f;
	float radius = 1.0f;

	//�g���Ă��邩�ǂ����̃t���O
    bool isUsed;
	bool isDelete;
};

