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

	//コンストラクタ
	GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input);

	//デストラクタ
	~GamePlay();

	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	//次のシーンへ
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



