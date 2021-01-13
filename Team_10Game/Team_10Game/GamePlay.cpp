#include "GamePlay.h"
#include "SafeDelete.h"
#include "Collision.h"
#include "Random.h"

//文字描画用
#include <sstream>
#include <iomanip>

GamePlay::GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input, Score* score,Sound* sound)
	:BaseScene(sceneChanger),
	dxManager(manager),
	input(input),
	score(score),
	sound(sound)
{

#pragma region モデルの読み込みと作成

	//スカイドームモデル
	/*skyDomeModel = new Model(dxManager->GetDevice());
	skyDomeModel->CreateModel("skydome");*/
	//球モデル
	sphereModel = new Model(dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");
	////グラウンドモデル
	//groundModel = new Model(dxManager->GetDevice());
	//groundModel->CreateModel("ground");

#pragma endregion

	debugText.Initialize(0);
	hpText.Initialie(0);
}


GamePlay::~GamePlay()
{
	//safedelete(skyDomeModel);
	safedelete(sphereModel);
	//safedelete(groundModel);
	safedelete(ground);
	//safedelete(skyDome);
	for(auto e:enemys)
	{
		safedelete(e);
	}
	safedelete(player);
	safedelete(particleMan);
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
	//skyDome = GameObject::Create();
	//skyDome->SetModel(skyDomeModel);

	////グラウンド
	//ground = GameObject::Create();
	//ground->SetModel(groundModel);
	//ground->SetPosition({ 0, -2, 0 });

	// パーティクルマネージャー生成
	particleMan = ParticleManager::Create();
	sound->PlayLoop("Gameplay");
#pragma endregion

}

void GamePlay::Update()
{
	score->Update();

	player->Update();

	for (auto& e : enemys)
	{
		e->Update(player->GetPosition());
	}

	//skyDome->Update();
	//ground->Update();

	//体力がなくなったら次のシーンへ
	if (player->GetHp() < 1)
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
				//スコアを追加
				score->AddScore(20);
				deadPos = enemys[j]->GetPosition();
			}
		}
	}

	//敵とプレイヤーの当たり判定
	for (int j = 0; j < enemys.size(); j++)
	{
		bool PtoEs = Collision::SphereToSphere(player->GetSphere(), enemys[j]->GetSphere());
		if (PtoEs)
		{
			enemys[j]->ChangeDeadFlag(true);
			player->Damage(1);
			deadPos = enemys[j]->GetPosition();
		}
	}
#pragma endregion

	//残り時間を表示
	std::ostringstream timerstr;
	timerstr.clear();
	///////////
	//timerstr << "Time:" << std::fixed << std::setprecision(1) << "Test";
	//debugText.Print(timerstr.str(), 800, 0, 5.0f);
	//////////
	//std::ostringstream hpstr;
	//hpstr.clear();
	//hpstr << "HP:" << std::fixed << std::setprecision(1) << hp;
	//hpText.Print(hpstr.str(), 200, 0, 5.0f);


	std::ostringstream hpstr;
	hpstr.clear();
	hpstr << "HP:" << std::fixed << std::setprecision(1) << player->GetHp();
	hpText.Print(hpstr.str(), 200, 0, 2.0f);

	for (auto& e : enemys)
	{
		if (e->GetDeadFlag())
		{
			for (int i = 0; i < 100; i++)
			{
				//X,Y,Z全ての座標で{-0.05f,+0.05f}でランダムに分布
				const float rnd_vel = 0.15f;
				XMFLOAT3 vel{};
				vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
				vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
				vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
				//重力に見立ててYのみ[-0.001f,0]でランダムに分布
				XMFLOAT3 acc{};
				const float rnd_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * rnd_acc;

				XMFLOAT4 color = { 1.0f, 0.0f, 0.0f, 0.0f };
				float t = 2;

				deadPos = { deadPos.x, deadPos.y + t, deadPos.z };

				//追加
				particleMan->Add(120, deadPos, vel, acc, 1.0f, 0.0f, color);
			}
		}
	}
	particleMan->Update();
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

	//skyDome->Draw();
	//ground->Draw();

	// 3Dオブジェクト描画後処理
	GameObject::EndDraw();

	//2Dテクスチャ描画前処理
	Sprite::BeginDraw(cmdList);

	score->Draw(400, 0, 8);
	debugText.DrawAll(cmdList);
	hpText.DrawAll(cmdList);

	//2Dテクスチャ描画後処理
	Sprite::EndDraw();

	//パーティクル描画
	ParticleManager::PreDraw(cmdList);

	particleMan->Draw();

	ParticleManager::PostDraw();
}

void GamePlay::NextScene()
{
	sceneChanger->ChangeScene(SceneEnding);
}
