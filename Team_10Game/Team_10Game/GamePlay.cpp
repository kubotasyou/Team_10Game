#include "GamePlay.h"
#include "SafeDelete.h"
#include "Collision.h"
#include "Random.h"

//文字描画用
#include <sstream>
#include <iomanip>

GamePlay::GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input, Score* score, Sound* sound)
	:BaseScene(sceneChanger),
	dxManager(manager),
	input(input),
	score(score),
	sound(sound)
{

#pragma region モデルの読み込みと作成

	//宇宙モデル
	spaceDomeModel = new Model(dxManager->GetDevice());
	spaceDomeModel->CreateModel("spacedome");
	//球モデル
	sphereModel = new Model(dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");
	////グラウンドモデル
	//groundModel = new Model(dxManager->GetDevice());
	//groundModel->CreateModel("ground");

	//プレイヤーテスト用モデル
	charaModel = new Model(dxManager->GetDevice());
	charaModel->CreateModel("chr_sword");

#pragma endregion

	debugText.Initialize(0);
	hpText.Initialie(0);


#pragma region 当たり判定テスト

	rect.x = 0.0f;
	rect.y = 0.0f;

	rect.width = 60.0f;
	rect.height = 60.0f;

	point.x = 50.0f;
	point.x = 50.0f;

	sphere.center = XMVectorSet(0, 0, 0, 1);
	sphere.radius = 1.0f;

#pragma endregion


}


GamePlay::~GamePlay()
{
	//safedelete(skyDomeModel);
	safedelete(sphereModel);
	//safedelete(groundModel);
	//safedelete(ground);
	//safedelete(skyDome);
	for (auto e : enemys)
	{
		safedelete(e);
	}
	safedelete(player);
	safedelete(particleMan);
}

void GamePlay::Initialize()
{

#pragma region オブジェクトの作成

	//プレイヤー
	player = new Player(input, charaModel);
	player->Initialize();

	//敵
	enemys.resize(enemyNum);
	for (int i = 0; i < enemyNum; i++)
	{
		enemys[i] = new Enemy(sphereModel);
		enemys[i]->Initialize(player->GetPosition());
	}

	//宇宙ドーム
	spaceDome = GameObject::Create();
	spaceDome->SetModel(spaceDomeModel);
	spaceDome->SetScale({ 0.5f,0.5f,0.5f });

	////グラウンド
	//ground = GameObject::Create();
	//ground->SetModel(groundModel);
	//ground->SetPosition({ 0, -2, 0 });

	// パーティクルマネージャー
	particleMan = ParticleManager::Create();
	sound->PlayLoopWav("Gameplay");

	//背景画像
	backGround = Sprite::Create(5, { 0,0 });

#pragma endregion

}

void GamePlay::Update()
{
	score->Update();

	//プレイヤー
	player->Update();

	//敵
	for (auto& e : enemys)
	{
		e->Update(player->GetPosition());
	}

	//宇宙ドーム
	spaceDome->Update();

	//グラウンド
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

	////何かしらの位置を表示
	//std::ostringstream somethingPos;
	//somethingPos.clear();
	//somethingPos << "CameraPos:" << std::fixed << std::setprecision(1) << player->GetPosition().x << player->GetPosition().y << player->GetPosition().z;
	//debugText.Print(somethingPos.str(), 0, 500, 5.0f);



	std::ostringstream hpstr;
	hpstr.clear();
	hpstr << "HP:" << std::fixed << std::setprecision(1) << player->GetHp();
	hpText.Print(hpstr.str(), 0, 0, 5.0f);

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

				//追加
				particleMan->Add(60, deadPos, vel, acc, 1.0f, 0.0f, color);
			}
		}
	}
	particleMan->Update();



//#pragma region 当たり判定テスト
//
//	/*bool ReToPo = Collision::CheckRectangleToDot(rect, point);*/
//	bool ReToPo = Collision::CheckRectangleToSphere(rect, sphere);
//	if (ReToPo)
//	{
//		std::ostringstream test;
//		test.clear();
//		test << "Rectangle&Dot...Hit!!:" << std::fixed;
//		hpText.Print(test.str(), 0, 400, 5.0f);
//	}
//	else
//	{
//		std::ostringstream test;
//		test.clear();
//		test << "Rectangle&Dot...No!!:" << std::fixed;
//		hpText.Print(test.str(), 0, 400, 5.0f);
//	}
//
//	if (input->GetKeyDown(KeyCode::A))
//	{
//		sphere.center.m128_f32[0] -= 0.1f;
//	}
//	else if (input->GetKeyDown(KeyCode::D))
//	{
//		sphere.center.m128_f32[0] += 0.1f;
//	}
//	if (input->GetKeyDown(KeyCode::S))
//	{
//		sphere.center.m128_f32[1] -= 0.1f;
//	}
//	else if (input->GetKeyDown(KeyCode::W))
//	{
//		sphere.center.m128_f32[1] += 0.1f;
//	}
//	if (input->GetKeyDown(KeyCode::DOWN))
//	{
//		sphere.center.m128_f32[2] -= 0.1f;
//	}
//	else if (input->GetKeyDown(KeyCode::UP))
//	{
//		sphere.center.m128_f32[2] += 0.1f;
//	}
//
//	//何かしらの位置を表示
//	std::ostringstream somethingPos;
//	somethingPos.clear();
//	somethingPos << "PointPos:" << 
//		std::fixed << 
//		std::setprecision(1) << 
//		sphere.center.m128_f32[0] << "," << 
//		sphere.center.m128_f32[1] << "," <<
//		sphere.center.m128_f32[2];
//
//	debugText.Print(somethingPos.str(), 0, 500, 5.0f);
//
//#pragma endregion

}

void GamePlay::Draw()
{

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();

	// 3Dオブジェクト描画前処理
	GameObject::BeginDraw(cmdList);

	//プレイヤー
	player->Draw();

	//敵
	for (auto& e : enemys)
	{
		e->Draw();
	}

	//宇宙ドーム
	spaceDome->Draw();

	//グラウンド
	//ground->Draw();

	// 3Dオブジェクト描画後処理
	GameObject::EndDraw();



	//パーティクル描画
	ParticleManager::PreDraw(cmdList);

	particleMan->Draw();

	ParticleManager::PostDraw();



	//2Dテクスチャ描画前処理
	Sprite::BeginDraw(cmdList);

	//backGround->Draw();
	score->Draw(800, 0, 5);
	debugText.DrawAll(cmdList);
	hpText.DrawAll(cmdList);

	//2Dテクスチャ描画後処理
	Sprite::EndDraw();




}

void GamePlay::NextScene()
{
	sound->StopWav();
	sceneChanger->ChangeScene(SceneEnding);
}
