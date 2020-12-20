#include "Player.h"

#include "SafeDelete.h"

#include <DirectXMath.h>

Player::Player(DirectXManager * dxManager, Input * input)
{
	this->dxManager = dxManager;
	this->input = input;

	//プレイヤー生成
	sphereModel = new Model(this->dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");
	player = GameObject::Create();
	player->SetModel(sphereModel);
	player->SetPosition(XMFLOAT3(0, 5, 0));

	//足元の球の生成
	collider = GameObject::Create();
	collider->SetModel(sphereModel);
	//足元となる位置に設置!
	collider->SetPosition(XMFLOAT3(player->GetPosition().x, player->GetPosition().y - player->GetRadius(), player->GetPosition().z));
	//サイズは小さめ
	collider->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	//色は赤色
	collider->SetColor({ 1,0,0,1 });
	//足元の球の半径を変更
	collider->SetRadius(colliderRadius);

	sound = new Sound();
	sound->LoadSE("Alarm01");

	timer = new CountDownTimer();
	timer->SetTime(1.5f);
}

Player::~Player()
{
	safe_delete(player);
	safe_delete(sphereModel);
	safe_delete(collider);
}

void Player::Initialize()
{
	velocity = { 0,0,0 };
}

void Player::Update()
{
	player->Update();
	collider->Update();

	Move();
	Jump();
}

void Player::Draw()
{
	player->Draw();
	collider->Draw();
}

void Player::Move()
{
	//これ必須
	velocity = { 0,0,0 };

	position = player->GetPosition();

	velocity.x += input->GetStick("Vertices") * speed;
	velocity.z += input->GetStick("Horizontal") * speed;

	//地面についていないとき
	if (!isGround)
	{
		player->SetColor({ 1,0,0,1 });

		//重力(仮)
		velocity.y -= 0.2f;

	}

	if (isJumpFlag)
	{
		timer->Update();

		velocity.y += 2.0f;

		if (timer->IsTime())
		{

		}
	}

	if(isGround)
	{
		//ジャンプしてない状態にする
		isJumpFlag = false;

		//ボタンを押したら
		if (!isJumpFlag && input->GetJoyPadTrigger(JoyPad::RB))
		{
			//ジャンプしている状態にする
			isJumpFlag = true;
		}
		player->SetColor({ 0,1,0,1 });
	}

	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);

	player->SetPosition(position);

	/*フレーム数をかける
	　一定時間上昇し続ける(その間重力はない)
	 　時間になったら上昇を終了し、重力を付ける。*/


	//常にプレイヤーの足元に設置
	collider->SetPosition(XMFLOAT3(player->GetPosition().x, player->GetPosition().y - player->GetRadius(), player->GetPosition().z));
}

void Player::Jump()
{
	if (input->GetJoyPadRelease(JoyPad::B))
	{
		sound->PlaySE("Alarm01", 0.2f);
	}
}
