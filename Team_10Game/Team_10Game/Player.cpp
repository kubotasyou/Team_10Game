#include "Player.h"
#include "SafeDelete.h"

#include <DirectXMath.h>

Player::Player(Input * input, Model* model)
{
	this->input = input;
	this->sphereModel = model;

	//プレイヤー生成
	player = GameObject::Create();
	player->SetModel(sphereModel);
}

Player::~Player()
{
//	safedelete(input);
//	safedelete(player);
//	safedelete(sphereModel);
	//for (auto test : bulletList)
	//{
	//	safedelete(test);
	//}
}

void Player::Initialize()
{
	//一応初期化
	velocity = { 0,0,0 };

	//位置初期化
	position = float3(0, 1, 0);
	player->SetPosition(position);
	//自機：当たり判定初期化
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);//位置
	sphere.radius = 1.0f;//半径
}

void Player::Update()
{
	player->Update();

	Move();

	//ボタンを押したら使用される
	if (input->GetKeyTrigger(KeyCode::Z))
	{
		Shot();
		//memo : ホントにメモ
		////リストの中を全検索して、フラグがfalseなのを探す。
		//for (auto test : bl)
		//{
		//	if (!test->GetUsedFlag())
		//	{
		//		//falseのフラグを1つ見つけたら、代入して処理を中断。
		//		break;
		//	}
		//	//falseのフラグを見つけたらtureに変える。
		//	test->ChangeUsed(true);
		//}

	}
	//リストの中を全検索して、フラグがtrueなのを探す。
	for (auto test : bulletList)
	{
		//tureのフラグを見つけたら更新する
		test->Update();
	}

}
void Player::Shot()
{
	bulletList.emplace_back(new Bullet(player->GetPosition(), sphereModel));
	for (int i = 0; i < bulletList.size(); i++)
	{
		//リスト内のフラグがfalseのものを探す
		if (!bulletList[i]->GetUsedFlag())
		{
			//falseのフラグを見つけたら、trueに変える
			bulletList[i]->ChangeUseFlag(true);
			break;
		}
	}
}

void Player::Draw()
{
	player->Draw();

	//リストの中を全検索して、フラグがtrueなのを探す。
	for (auto test : bulletList)
	{
		//trueのフラグを見つけたら更新する
		test->Draw();
	}
}

void Player::Move()
{
	//これ必須
	velocity = { 0,0,0 };

	position = player->GetPosition();

	velocity.x += input->GetStick("Vertices") * speed;
	velocity.y += input->GetStick("Horizontal") * speed;

	//float3に+=のoperatorがない。
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;

	//当たり判定も一緒に動かす
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);

	player->SetPosition(position);
}
