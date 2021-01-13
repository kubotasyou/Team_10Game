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
#include "Score.h"
#include "HpText.h"

#include "ParticleManager.h"

class GamePlay :public BaseScene
{
public:
	//コンストラクタ
	GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input, Score* score);

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
	Score* score;

	Model* skyDomeModel;//スカイドームモデル
	Model* sphereModel; //球モデル
	Model* groundModel; //グラウンドモデル

	GameObject* skyDome;//スカイドームオブジェクト
	GameObject* ground;	//グラウンドオブジェクト

	Player* player;
	std::vector<Enemy*> enemys;
	int enemyNum = 10;//一度に表示する敵の数。
	bool hit = false;//多段ヒットしないように条件追加
	bool timer = false;//追加
	int t = 0;//追加
	int d = 1;//追加
	DebugText debugText;
	HpText hpText;
	//memo : デバッグテキストが使えないので、Spriteクラスを確認する

	ParticleManager* particleMan = nullptr;
	DirectX::XMFLOAT3 deadPos;
};



