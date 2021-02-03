#include "Bullet.h"
#include "SafeDelete.h"

Bullet::Bullet(float3 position, Model* model,float3 pointerposition)
{
	//�e����
	this->sphereModel = model;
	bullet = GameObject::Create();//�I�u�W�F�N�g����
	bullet->SetModel(sphereModel);//���f���Z�b�g
	bullet->SetPosition(position);//�ʒu������
	bullet->SetScale({ 0.5f, 0.5f, 0.5f });//�傫�������������Ă���
	bullet->SetColor({0,0,1,1});
	//�g���Ă��Ȃ�
	isUsed = false;
	isDeadFlag = false;


#pragma region �����蔻�菉����

	//�ʒu
	sphere.center = XMVectorSet(position.x, position.y, position.z, 1);
	//���a
	sphere.radius = 0.8f;
	this->pointerPosition = pointerposition;
#pragma endregion


}

Bullet::~Bullet()
{
	safedelete(bullet);
	safedelete(sphereModel);
}

void Bullet::Update()
{
	//�g���ĂȂ���Ώ������Ȃ�
	if (!isUsed) return;

	bullet->Update();
	if (position.z > 12)
	{
		isDeadFlag = true;
	}

#pragma region ���}���u

	//�e�̏��������ł��Ȃ��ƁA�����蔻������Z�b�g�ł��Ȃ��B
	//���̂��߁A�e���������Ƃ��̓v���C���[�̈ʒu�ɓ����蔻���u��

	if (isDeadFlag)
	{
		//�ʒu������������
		sphere.center = { 0, 0, 0, 1 };
		//���a������������
		sphere.radius = 0.0f;
	}

#pragma endregion

	//�e�̈ړ�
	Move();
}

void Bullet::Draw()
{
	//�g���ĂȂ���Ώ������Ȃ�
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
	//����K�{
	velocity = { 0,0,0 };

	//�ʒu�擾
	position = bullet->GetPosition();

	//���ł����[
	dist.x = pointerPosition.x - position.x;
	dist.y = pointerPosition.y - position.y;
	dist.z = pointerPosition.z - position.z;
	distance = sqrtf(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
	velocity.x = dist.x / distance * speed;
	velocity.y = dist.y / distance * speed;
	velocity.z = dist.z / distance * speed;

	//x,y����Ȃ����ǂ���������
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;

	//�����蔻����ꏏ�ɓ�����
	sphere.center = XMVectorSet(position.x, position.y, position.z, 1);

	//�ʒu�𔽉f
	bullet->SetPosition(position);
}
