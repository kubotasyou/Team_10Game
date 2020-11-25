#pragma once
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "Object3D.h"
#include "GameObject.h"
#include "Model.h"
#include "DebugText.h"

#include "CountDownTimer.h"

#include "Sound.h"

class GamePlay
{
public:

	//コンストラクタ
	GamePlay();

	//デストラクタ
	~GamePlay();

	//初期化
	void Initialize(DirectXManager* dxManager, Input* input);

	//更新
	void Update();

	//描画
	void Draw();

private:

	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;

	DebugText debugText;

	Sprite* sprite = nullptr;
	GameObject* obj = nullptr;
	Model* mod = nullptr;
	
	GameObject * chara = nullptr;
	Model * charaModel = nullptr;

	GameObject * chara2 = nullptr;
	Model * charaModel2 = nullptr;


	XMFLOAT3 gNormal;

	CountDownTimer* downTimer;

	Sound* sound = nullptr;
};

