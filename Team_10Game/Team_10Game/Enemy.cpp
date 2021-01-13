#include "Enemy.h"
#include "SafeDelete.h"
#include "Random.h"
#include <assert.h>

Enemy::Enemy(Model * model)
{
	this->enemyModel = model;

	//�G����
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
	//�����ʒu
	enemy->SetPosition(position);
	enemy->SetScale({ 0.5f,0.5f,0.5f });
	//���@�F�����蔻�菉����
	sphere.center = XMVectorSet(position.x, position.y, position.z, 1);//�ʒu
	sphere.radius = 0.5f;//���a

	playerPosition = { 0, 1, -5 };


	isDead = false;
}

void Enemy::Update()
{
	enemy->Update();
	Move();

	//�ʒu�擾
	position = enemy->GetPosition();
	//�����蔻����ꏏ�ɓ�����
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);

	//����ł�Ƃ��̏����B
	if (isDead)
	{
		enemy->SetColor({ 1, 0, 0, 1 });
		deadPos = enemy->GetPosition();
		//�ʒu�A�^�C�}�[�A�t���O��������
		Initialize();
	}
	else
	{
		enemy->SetColor({ 1, 1, 1, 1 });
	}
	///////////��ʉ��܂ōs�����������悤��
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
	//		//X,Y,Z�S�Ă̍��W��{-0.05f,+0.05f}�Ń����_���ɕ��z
	//		const float rnd_vel = 0.1f;
	//		XMFLOAT3 vel{};
	//		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
	//		XMFLOAT3 acc{};
	//		const float rnd_acc = 0.001f;
	//		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	//		XMFLOAT4 color = { 0.0f, 1.0f, 0.0f, 0.0f };

	//		//�ǉ�
	//		particleMan->Add(120, deadPos, vel, acc, 1.0f, 0.0f, color);
	//	}
	//}
}

void Enemy::Draw()
{
	//����ł����珈�����Ȃ��B
	if (isDead) return;

	enemy->Draw();
}

void Enemy::ChangeDeadFlag(bool value)
{
	isDead = value;
}

void Enemy::Move()
{
	//����ł����珈�����Ȃ��B
	if (isDead) return;

	velocity = { 0,0,0 };

	//�ʒu�擾
	position = enemy->GetPosition();

#pragma region �v���C���[�Ƃ̋������v�Z���Ĉړ��ʂɑ��

	dist.x = playerPosition.x - position.x;
	dist.y = playerPosition.y - position.y;
	dist.z = playerPosition.z - position.z;
	distance = sqrtf(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
	velocity.x = dist.x / distance * speed;
	velocity.y = dist.y / distance * speed;
	velocity.z = dist.z / distance * speed;

#pragma endregion

	//float3��+=��operator���Ȃ��B
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;

	//�����蔻����ꏏ�ɓ�����
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);

	//�ʒu�𔽉f�B�萔�o�b�t�@�[�r���[�ɓ]��
	enemy->SetPosition(position);
}
