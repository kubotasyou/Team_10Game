#include "Bullet.h"

#include "SafeDelete.h"

Bullet::Bullet(DirectXManager * dxManager, XMFLOAT3 position)
{
	this->dxManager = dxManager;

	//�e����
	sphereModel = new Model(this->dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");//���f������
	bullet = GameObject::Create();      //�I�u�W�F�N�g����
	bullet->SetModel(sphereModel);      //���f���Z�b�g
	bullet->SetPosition(position);      //�ʒu������

	//�g���Ă��Ȃ�
	isUsed = false;
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
	//�g���ĂȂ���Ώ������Ȃ�
	if(!isUsed) return;

	bullet->Update();


	//����K�{
	velocity = { 0,0,0 };
	position = bullet->GetPosition();
	//���ł����[
	velocity.z += speed;
	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);
	bullet->SetPosition(position);
}

void Bullet::Draw()
{
	//�g���ĂȂ���Ώ������Ȃ�
	if(!isUsed) return;

	bullet->Draw();
}

void Bullet::ChangeUsed(bool flag)
{
	isUsed = flag;
}
