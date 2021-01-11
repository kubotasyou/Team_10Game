#include "GamePlay.h"
#include "SafeDelete.h"
#include "Collision.h"

//文字描画用
#include <sstream>
#include <iomanip>

GamePlay::GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input)
	:BaseScene(sceneChanger),
	dxManager(manager),
	input(input)
{

#pragma region モデルの読み込みと作成

	//スカイドームモデル
	skyDomeModel = new Model(dxManager->GetDevice());
	skyDomeModel->CreateModel("skydome");
	//球モデル
	sphereModel = new Model(dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");
	//グラウンドモデル
	groundModel = new Model(dxManager->GetDevice());
	groundModel->CreateModel("ground");

#pragma endregion

	debugText.Initialize(0);

}


GamePlay::~GamePlay()
{
	safedelete(skyDomeModel);
	safedelete(sphereModel);
	safedelete(groundModel);
	safedelete(ground);
	safedelete(skyDome);
	for(auto e:enemys)
	{
		safedelete(e);
	}
	safedelete(player);
}

void GamePlay::Initialize()
{

#pragma region 今からやるやつ

	//プレイヤー
	player = new Player(input, sphereModel);
	player->Initialize();

	//敵
	enemys.resize(enemyNum);
	for (int i = 0; i < enemyNum; i++)
	{
		enemys[i] = new Enemy(sphereModel);
		enemys[i]->Initialize();
	}

	//スカイドーム
	skyDome = GameObject::Create();
	skyDome->SetModel(skyDomeModel);

	//グラウンド
	ground = GameObject::Create();
	ground->SetModel(groundModel);
	ground->SetPosition({ 0, -2, 0 });

#pragma endregion

}

void GamePlay::Update()
{
	player->Update();

	for (auto& e : enemys)
	{
		e->Update();
	}

	skyDome->Update();
	ground->Update();

	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		NextScene();
	}

#pragma region 当たり判定処理

	//弾を全検索
	for (int i = 0; i < player->GetBulletList().size(); i++)
	{
		//敵を全検索
		for (int j = 0; j < enemys.size(); j++)
		{
			bool BstoEs = Collision::SphereToSphere(player->GetBulletList()[i]->GetSphere(), enemys[j]->GetSphere());
			if (BstoEs)
			{
				//弾を削除
				player->GetBulletList()[i]->ChangeDeadFlag(true);
				//敵を削除
				enemys[j]->ChangeDeadFlag(true);
			}
		}
	}

#pragma endregion

	//残り時間を表示
	std::ostringstream timerstr;
	timerstr.clear();
	timerstr << "Time:" << std::fixed << std::setprecision(1) << "Test";
	debugText.Print(timerstr.str(), 800, 0, 5.0f);

}

void GamePlay::Draw()
{

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();
	// 3Dオブジェクト描画前処理
	GameObject::BeginDraw(cmdList);

	player->Draw();
	for (auto& e : enemys)
	{
		e->Draw();
	}

	skyDome->Draw();
	ground->Draw();

	// 3Dオブジェクト描画後処理
	GameObject::EndDraw();

	Sprite::BeginDraw(cmdList);

	debugText.DrawAll(cmdList);

	Sprite::EndDraw();
}

void GamePlay::NextScene()
{
	sceneChanger->ChangeScene(SceneEnding);
}
