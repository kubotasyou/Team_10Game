#include "GamePlay.h"
#include "SafeDelete.h"

#include "Collision.h"


#pragma comment(lib, "xaudio2.lib")

#include <sstream>
#include <iomanip>

GamePlay::GamePlay()
{

}

GamePlay::~GamePlay()
{
	safe_delete(sprite);
	safe_delete(groundModel);
	safe_delete(ground);
	safe_delete(charaModel);
	safe_delete(chara);
	safe_delete(charaModel2);
	safe_delete(chara2);
	safe_delete(player);
}

void GamePlay::Initialize(DirectXManager * dxManager, Input * input)
{
	this->dxManager = dxManager;
	this->input = input;

	////テクスチャ読み込み
	//Sprite::LoadTexture(2, L"Resources/Texture/kirby.png");
	////オブジェクト生成
	//sprite = Sprite::Create(2, { -1.0f,0.0f });


	//readme : 1サウンドクラスを使いやすいようにする。
	//readme : レイの当たり判定は使えそうだから作っておく。
	//readme : カメラを分離させる。
	//readme : シーン遷移ちゃんとしたやつ
	//readme : プレイヤー→キャラクター管理クラス→エネミー



	groundModel = new Model(dxManager->GetDevice());
	groundModel->CreateModel("ground");
	ground = GameObject::Create();
	ground->SetModel(groundModel);

	gNormal = XMFLOAT3(0, 1, 0);


	charaModel = new Model(dxManager->GetDevice());
	charaModel->CreateModel("sphere2");
	chara = GameObject::Create();
	chara->SetModel(charaModel);
	chara->SetPosition(XMFLOAT3(0, 5, 0));

	charaModel2 = new Model(dxManager->GetDevice());
	charaModel2->CreateModel("sphere2");
	chara2 = GameObject::Create();
	chara2->SetModel(charaModel);
	chara2->SetPosition(XMFLOAT3(2.f, 5, 0));

	downTimer = new CountDownTimer();
	downTimer->SetTime(5.0f);

	sound = new Sound();
	sound->LoadSound("Alarm01");
	//sound->Play("Alarm01",0.1f);

	//sound->LoadSound("3MinutesCooking");
	//sound->Play("3MinutesCooking",0.1f);

	sound->LoadSound("GodisSaying");
	//sound->PlayLoop("GodisSaying", 0.1f);


	player = new Player(dxManager, input);
	player->Initialize();
}

void GamePlay::Update()
{
#pragma region カメラの移動

	if (input->GetKeyDown(KeyCode::A))
	{
		GameObject::CameraMoveVector(XMFLOAT3(-0.1f, 0.0f, 0.0f));
	}
	if (input->GetKeyDown(KeyCode::D))
	{
		GameObject::CameraMoveVector(XMFLOAT3(+0.1f, 0.0f, 0.0f));
	}
	if (input->GetKeyDown(KeyCode::W))
	{
		GameObject::CameraMoveVector(XMFLOAT3(0.0f, 0.0f, 0.1f));
	}
	if (input->GetKeyDown(KeyCode::S))
	{
		GameObject::CameraMoveVector(XMFLOAT3(0.0f, 0.0f, -0.1f));
	}

#pragma endregion

	//ReadMe : プレイヤーにカメラを追従させたい

#pragma region プレイヤーと地面

	//プレイヤーと地面
	bool isGround = Collision::CheckSphereToPanel(player->GetColliderPos(), player->GetColliderRadius() , gNormal, 0.0f);

	/*if (isGround)
	{*/
		//ReadMe : ここに当たった時の処理を書く

		//プレイヤーに当たった判定を返す
		player->SetisGround(isGround);
	//}

#pragma endregion

#pragma region 球と球の判定

	//球と球
	bool hit2 = Collision::CircleToCircle(chara->GetPosition(), chara->GetRadius(), chara2->GetPosition(), chara2->GetRadius());

	if (hit2)
	{
		//ReadMe : ここに当たった時のd処理を書く
		chara->SetColor(XMFLOAT4(1, 0, 0, 1));
	}

#pragma endregion


	if (!isGround && !hit2)
	{
		chara->SetColor(XMFLOAT4(1, 1, 1, 1));
	}

	if (input->GetKeyTrigger(KeyCode::Z))
	{
		sound->Play("GodisSaying", 0.1f);
	}
	if (input->GetJoyPadRelease(JoyPad::A))
	{
		sound->Play("Alarm01", 0.1f);
	}

#pragma region 時間になったら系

	//時間になったらモデルチェンジ
	if (downTimer->IsTime())
	{
		//sound->Play("Alarm01");
	}

#pragma endregion

	chara->SetTarget(chara->GetPosition());

	player->Update();
	ground->Update();
	//chara->Update();
	//chara2->Update();
	downTimer->Update();
}

void GamePlay::Draw()
{
	Sprite::BeginDraw(dxManager->GetcmdList());
	//sprite->Draw();
	Sprite::EndDraw();

	GameObject::BeginDraw(dxManager->GetcmdList());
	ground->Draw();
	//chara->Draw();
	//chara2->Draw();
	player->Draw();
	GameObject::EndDraw();
}
