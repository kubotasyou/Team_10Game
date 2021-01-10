#pragma once
#include "BaseScene.h"
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "Sound.h"

class Ending : public BaseScene
{
public:
	//コンスト
	Ending(ISceneChanger* sceneChanger, DirectXManager* dxcommon, Input* input,Sound* sound);
	//デスト
	~Ending();
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//次のシーンへ
	void NextScene();

private:
	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;
	Sprite* spriteBG = nullptr;
	Sound* sound = nullptr;

	float test = 0;
};

