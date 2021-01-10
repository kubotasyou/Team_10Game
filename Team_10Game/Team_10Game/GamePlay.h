#pragma once
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "Object3D.h"
#include "GameObject.h"
#include "Model.h"
#include "CountDownTimer.h"
#include "Sound.h"
#include "BaseScene.h"
#include "Player.h"

class GamePlay :public BaseScene
{
public:

	//�R���X�g���N�^
	GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input);

	//�f�X�g���N�^
	~GamePlay();

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//���̃V�[����
	void NextScene();
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



