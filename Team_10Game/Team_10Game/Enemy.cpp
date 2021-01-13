#include "Enemy.h"
#include "SafeDelete.h"
#include "Random.h"
#include <assert.h>

Enemy::Enemy(Model * model)
{
	this->enemyModel = model;

	//敵生成
	enemy = GameObject::Create();
	enemy->SetModel(enemyModel);
}

Enemy::~Enemy()
{
    //safedelete(enemyModel);
	safedelete(enemy);
}

void Enemy::Initialize()
{
	position = { Random::SetRandom(-5,5),Random::SetRandom(-5,5),55.0f };
	//初期位置
	enemy->SetPosition(position);
	enemy->SetScale({ 0.5f,0.5f,0.5f });
	//自機：当たり判定初期化
	sphere.center = XMVectorSet(position.x, position.y, position.z, 1);//位置
	sphere.radius = 0.5f;//半径

	playerPosition = { 0, 1, -5 };


	isDead = false;
}

void Enemy::Update()
{
	enemy->Update();
	Move();

	//位置取得
	position = enemy->GetPosition();
	//当たり判定も一緒に動かす
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);

	//死んでるときの処理。
	if (isDead)
	{
		enemy->SetColor({ 1, 0, 0, 1 });
		deadPos = enemy->GetPosition();
		//位置、タイマー、フラグを初期化
		Initialize();
	}
	else
	{
		enemy->SetColor({ 1, 1, 1, 1 });
	}
	///////////画面奥まで行ったら消えるように
	if (position.z <= -5)
	{
		isDead = true;
	}
	/////////////////////////////////////////


	//if (isDead)
	//{
	//	particleMan->Update();

	//	for (int i = 0; i < 5; i++)
	//	{
	//		//X,Y,Z全ての座標で{-0.05f,+0.05f}でランダムに分布
	//		const float rnd_vel = 0.1f;
	//		XMFLOAT3 vel{};
	//		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
	//		XMFLOAT3 acc{};
	//		const float rnd_acc = 0.001f;
	//		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	//		XMFLOAT4 color = { 0.0f, 1.0f, 0.0f, 0.0f };

	//		//追加
	//		particleMan->Add(120, deadPos, vel, acc, 1.0f, 0.0f, color);
	//	}
	//}
}

void Enemy::Draw()
{
	//死んでいたら処理しない。
	if (isDead) return;

	enemy->Draw();
}

void Enemy::ChangeDeadFlag(bool value)
{
	isDead = value;
}

void Enemy::Move()
{
	//死んでいたら処理しない。
	if (isDead) return;

	velocity = { 0,0,0 };

	//位置取得
	position = enemy->GetPosition();

#pragma region プレイヤーとの距離を計算して移動量に代入

	dist.x = playerPosition.x - position.x;
	dist.y = playerPosition.y - position.y;
	dist.z = playerPosition.z - position.z;
	distance = sqrtf(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
	velocity.x = dist.x / distance * speed;
	velocity.y = dist.y / distance * speed;
	velocity.z = dist.z / distance * speed;

#pragma endregion

	//float3に+=のoperatorがない。
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;

	//当たり判定も一緒に動かす
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);

	//位置を反映。定数バッファービューに転送
	enemy->SetPosition(position);
}
