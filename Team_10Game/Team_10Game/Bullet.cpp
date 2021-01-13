#include "Bullet.h"
#include "SafeDelete.h"

Bullet::Bullet(float3 position, Model* model)
{
	//弾生成
	this->sphereModel = model;
	bullet = GameObject::Create();//オブジェクト生成
	bullet->SetModel(sphereModel);//モデルセット
	bullet->SetPosition(position);//位置初期化
	bullet->SetScale({ 0.5f, 0.5f, 0.5f });//大きさを小さくしておく

	//使っていない
	isUsed = false;
	isDeadFlag = false;


#pragma region 当たり判定初期化

	//位置
	sphere.center = XMVectorSet(position.x, position.y, position.z, 1);
	//半径
	sphere.radius = 0.5f;

#pragma endregion


}

Bullet::~Bullet()
{
	safedelete(bullet);
	safedelete(sphereModel);
}

void Bullet::Update()
{
	//使われてなければ処理しない
	if (!isUsed) return;

	bullet->Update();
	if (position.z > 15)
	{
		isDeadFlag = true;
	}

#pragma region 応急処置

	//弾の初期化ができないと、当たり判定をリセットできない。
	//そのため、弾が消えたときはプレイヤーの位置に当たり判定を置く

	if (isDeadFlag)
	{
		//位置を初期化して
		sphere.center = { 0, 0, 0, 1 };
		//半径も小さくする
		sphere.radius = 0.0f;
	}

#pragma endregion

	//弾の移動
	Move();
}

void Bullet::Draw()
{
	//使われてなければ処理しない
	if (!isUsed) return;
	if (!isDeadFlag)
	{
		bullet->Draw();
	}
}

void Bullet::ChangeUseFlag(bool flag)
{
	isUsed = flag;
}

void Bullet::ChangeDeadFlag(bool flag)
{
	isDeadFlag = flag;
}

void Bullet::Move()
{
	//これ必須
	velocity = { 0,0,0 };

	//位置取得
	position = bullet->GetPosition();

	//飛んでいくー
	velocity.z += speed;

	//x,yいらないけどいちおうね
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;

	//当たり判定も一緒に動かす
	sphere.center = XMVectorSet(position.x, position.y, position.z, 1);

	//位置を反映
	bullet->SetPosition(position);
}
