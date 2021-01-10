#include "Bullet.h"
#include "SafeDelete.h"

Bullet::Bullet( XMFLOAT3 position, Model* model)
{
	//�e����
	this->sphereModel = model;
	bullet = GameObject::Create();      //�I�u�W�F�N�g����
	bullet->SetModel(sphereModel);      //���f���Z�b�g
	bullet->SetPosition(position);      //�ʒu������

	//�g���Ă��Ȃ�
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
	//�g���ĂȂ���Ώ������Ȃ�
	if (!isUsed) return;
	bullet->Update();
	if (position.z > 10)
	{
		isDelete = true;
	}
	//����K�{
	velocity = { 0,0,0 };
	position = bullet->GetPosition();
	//playerposition = player->GetPosition();
	//���ł����[
	velocity.z += speed;
	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);
	bullet->SetPosition(position);
}

void Bullet::Draw()
{
	//�g���ĂȂ���Ώ������Ȃ�
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