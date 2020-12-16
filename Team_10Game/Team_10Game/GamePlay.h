#pragma once
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "Object3D.h"
#include "GameObject.h"
#include "Model.h"
#include "CountDownTimer.h"
#include "Sound.h"

#include "Player.h"

class GamePlay
{
public:

	//�R���X�g���N�^
	GamePlay();

	//�f�X�g���N�^
	~GamePlay();

	//������
	void Initialize(DirectXManager* dxManager, Input* input);

	//�X�V
	void Update();

	//�`��
	void Draw();

private:

	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;

	Sprite* sprite = nullptr;
	GameObject* obj = nullptr;
	Model* mod = nullptr;
	GameObject* ground = nullptr;
	Model* groundModel = nullptr;
	
	GameObject * chara = nullptr;
	Model * charaModel = nullptr;

	GameObject * chara2 = nullptr;
	Model * charaModel2 = nullptr;

	GameObject * bullet[100];
	Model * charaModel3 = nullptr;


	XMFLOAT3 gNormal;

	CountDownTimer* downTimer;

	Sound* sound = nullptr;

	Player* player = nullptr;
};

