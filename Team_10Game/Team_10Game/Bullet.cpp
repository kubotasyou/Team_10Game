#include "Bullet.h"
#include "SafeDelete.h"

Bullet::Bullet( XMFLOAT3 position, Model* model)
{
	//弾生成
	this->sphereModel = model;
	bullet = GameObject::Create();      //オブジェクト生成
	bullet->SetModel(sphereModel);      //モデルセット
	bullet->SetPosition(position);      //位置初期化

	//使っていない
	isUsed = false;
	isDelete = false;
}

Bullet::~Bullet()
{
	safe_delete(bullet);
	safe_delete(sphereModel);
}

void Bullet::Update()
{
	//使われてなければ処理しない
	if (!isUsed) return;
	bullet->Update();
	if (position.z > 10)
	{
		isDelete = true;
	}
	//これ必須
	velocity = { 0,0,0 };
	position = bullet->GetPosition();
	//playerposition = player->GetPosition();
	//飛んでいくー
	velocity.z += speed;
	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);
	bullet->SetPosition(position);
}

void Bullet::Draw()
{
	//使われてなければ処理しない
	if (!isUsed) return;
	if (!isDelete)
	{
		bullet->Draw();
	}
}

void Bullet::ChangeUsed(bool flag)
{
	isUsed = flag;
}

void Bullet::ChangeDelete(bool flag)
{
	isDelete = flag;
}