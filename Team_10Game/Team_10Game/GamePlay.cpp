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
	safe_delete(mod);
	safe_delete(obj);
	safe_delete(charaModel);
	safe_delete(chara);
	safe_delete(charaModel2);
	safe_delete(chara2);
}

void GamePlay::Initialize(DirectXManager * dxManager, Input * input)
{
	this->dxManager = dxManager;
	this->input = input;

	Sprite::LoadTexture(0, L"Resources/Texture/debugfont.png");
	debugText.Initialize(0);

	////テクスチャ読み込み
	//Sprite::LoadTexture(2, L"Resources/Texture/kirby.png");
	////オブジェクト生成
	//sprite = Sprite::Create(2, { -1.0f,0.0f });

	//readme : カメラを分離させる。
	//readme : シーン遷移ちゃんとしたやつ
	//readme : プレイヤー→キャラクター管理クラス→エネミー



    mod = new Model(dxManager->GetDevice());
    mod->CreateModel("ground");
    obj = GameObject::Create();
    obj->SetModel(mod);

	gNormal = XMFLOAT3(0, 1, 0);

	charaModel = new Model(dxManager->GetDevice());
	charaModel->CreateModel("sphere2");
	chara = GameObject::Create();
	chara->SetModel(charaModel);
	chara->SetPosition(XMFLOAT3(-2,5,0));

	charaModel2 = new Model(dxManager->GetDevice());
	charaModel2->CreateModel("sphere2");
	chara2 = GameObject::Create();
	chara2->SetModel(charaModel);
	chara2->SetPosition(XMFLOAT3(2, 5, 0));

	downTimer = new CountDownTimer();
	downTimer->SetTime(5.0f);
	sound = new Sound();
	sound->Play("Resources/Sound/Alarm01.wav");
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

#pragma region オブジェクトの移動

	if (input->GetKeyDown(KeyCode::RIGHT) ||
		input->GetKeyDown(KeyCode::LEFT) ||
		input->GetKeyDown(KeyCode::UP) ||
		input->GetKeyDown(KeyCode::DOWN))
	{
		XMFLOAT3 position = chara->GetPosition();

		if (input->GetKeyDown(KeyCode::RIGHT))
		{
			position.x += 0.1f;
		}
		else if (input->GetKeyDown(KeyCode::LEFT))
		{
			position.x -= 0.1f;
		}
		else if (input->GetKeyDown(KeyCode::UP))
		{
			position.y += 0.1f;
		}
		else if (input->GetKeyDown(KeyCode::DOWN))
		{
			position.y -= 0.1f;

		}

		chara->SetPosition(position);
	}

#pragma endregion

	//ReadMe : プレイヤーにカメラを追従させたい

#pragma region 球と地面

	//球と地面
	bool hit = Collision::CheckSphereToPanel(chara->GetPosition(), chara->GetRadius(), gNormal, 0.0f);

	if (hit)
	{
		//ReadMe : ここに当たった時のd処理を書く
		chara->SetColor(XMFLOAT4(1, 0, 0, 1));
	}

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

	if (!hit && !hit2)
	{
		chara->SetColor(XMFLOAT4(1, 1, 1, 1));
	}

#pragma region 時間になったら系

	//時間になったらモデルチェンジ
	if (downTimer->IsTime())
	{
		//chara2->SetModel(mod);
	}

#pragma endregion

	chara->SetTarget(chara->GetPosition());

	obj->Update();
	chara->Update();
	chara2->Update();
	downTimer->Update();
}

void GamePlay::Draw()
{
	Sprite::BeginDraw(dxManager->GetcmdList());
	//sprite->Draw();
	Sprite::EndDraw();

	GameObject::BeginDraw(dxManager->GetcmdList());
	obj->Draw();
	chara->Draw();
	chara2->Draw();
	GameObject::EndDraw();

	debugText.DrawAll(dxManager->GetcmdList());
}
