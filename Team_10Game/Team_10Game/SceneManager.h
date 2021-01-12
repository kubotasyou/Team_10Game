#pragma once
#include "Input.h"
#include "Sound.h"
#include "Fader.h"
#include "DirectXManager.h"
#include "BaseScene.h"
#include "GameObject.h"
#include "Score.h"

class SceneManager : public ISceneChanger
{
public:
	//コンスト
	SceneManager(DirectXManager * manager, Input * input, Sound* sound);
	//デスト
	~SceneManager();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//シーン切り替え(次のシーン名)
	void ChangeScene(SceneType nextScene) override;

private:
	//memo 変数名の先頭に[m]を付けるのは、このクラスのメンバ変数だとわかりやすくするため

	BaseScene* currentScene;//現在のシーン
	SceneType nextScene;	//次回のシーン
	DirectXManager* dxManager;
	Input* input;
	Sound* sound;
	Fader* fader;
	Score* score;
};


