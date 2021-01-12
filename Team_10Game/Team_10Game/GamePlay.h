#pragma once
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Model.h"
#include "CountDownTimer.h"
#include "Sound.h"
#include "BaseScene.h"
#include "Player.h"
#include "Enemy.h"

#include "DebugText.h"
#include "HpText.h"

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

	DirectXManager* dxManager;
	Input* input;

	Model* skyDomeModel;//スカイドームモデル
	Model* sphereModel; //球モデル
	Model* groundModel; //グラウンドモデル

	GameObject* skyDome;//スカイドームオブジェクト
	GameObject* ground;	//グラウンドオブジェクト

	Player* player;
	std::vector<Enemy*> enemys;
	int enemyNum = 10;//一度に表示する敵の数。

	DebugText debugText;
	HpText hpText;
	//memo : デバッグテキストが使えないので、Spriteクラスを確認する

};



