#include "Bullet.h"

#include "SafeDelete.h"

Bullet::Bullet(DirectXManager * dxManager, XMFLOAT3 position)
{
	this->dxManager = dxManager;

	//弾生成
	sphereModel = new Model(this->dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");//モデル生成
	bullet = GameObject::Create();      //オブジェクト生成
	bullet->SetModel(sphereModel);      //モデルセット
	bullet->SetPosition(position);      //位置初期化

	//使っていない
	isUsed = false;
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
	//使われてなければ処理しない
	if(!isUsed) return;

	bullet->Update();


	//これ必須
	velocity = { 0,0,0 };
	position = bullet->GetPosition();
	//飛んでいくー
	velocity.z += speed;
	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);
	bullet->SetPosition(position);
}

void Bullet::Draw()
{
	//使われてなければ処理しない
	if(!isUsed) return;

	bullet->Draw();
}

void Bullet::ChangeUsed(bool flag)
{
	isUsed = flag;
}
