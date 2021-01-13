#include "Player.h"
#include "SafeDelete.h"

#include <DirectXMath.h>

Player::Player(Input * input, Model* model)
{
	this->input = input;
	this->sphereModel = model;

	//�v���C���[����
	player = GameObject::Create();
	player->SetModel(sphereModel);
}

Player::~Player()
{
    //safedelete(input);
    //safedelete(player);
    //safedelete(sphereModel);
	//for (auto test : bulletList)
	//{
	//	safedelete(test);
	//}
}

void Player::Initialize()
{
	//�ꉞ������
	velocity = { 0,0,0 };

	//�ʒu������
	position = float3(0, 1, 0);
	player->SetPosition(position);
	//���@�F�����蔻�菉����
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);//�ʒu
	sphere.radius = 1.0f;//���a

	//�̗͏�����
	hp = 5;

	downTimer = new CountDownTimer();
	downTimer->SetTime(noDamageTime);
}

void Player::Update()
{
	player->Update();
	//downTimer->Update();

	Move();

	//�{�^������������g�p�����
	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		Shot();
		//memo : �z���g�Ƀ���
		////���X�g�̒���S�������āA�t���O��false�Ȃ̂�T���B
		//for (auto test : bl)
		//{
		//	if (!test->GetUsedFlag())
		//	{
		//		//false�̃t���O��1��������A������ď����𒆒f�B
		//		break;
		//	}
		//	//false�̃t���O����������ture�ɕς���B
		//	test->ChangeUsed(true);
		//}

	}
	//���X�g�̒���S�������āA�t���O��true�Ȃ̂�T���B
	for (auto test : bulletList)
	{
		//ture�̃t���O����������X�V����
		test->Update();
	}

	//�_���[�W���󂯂Ă���Ƃ�
	if (dFlag == true)
	{
		//���Ԃ�i�߂�
		downTimer->Update();

		//�J�E���g�𑝂₷
		count++;

		//�J�E���g��10�Ŋ���؂��Ƃ�
		if (count % 10 == 0)
		{
			//�F��ς���
			player->SetColor({ 1, 0, 0, 1 });
		}
		else
		{
			//����؂�Ȃ��Ƃ��͒ʏ�F
			player->SetColor({ 1, 1, 1, 1 });
		}

		//���ԂɂȂ�����
		if (downTimer->IsTime())
		{
			player->SetColor({ 1, 1, 1, 1 });
			//�_���[�W���󂯂Ă��Ȃ���Ԃɂ���
			count = 0;
			dFlag = false;
		}
	}

}
void Player::Shot()
{
	bulletList.emplace_back(new Bullet(player->GetPosition(), sphereModel));
	for (int i = 0; i < bulletList.size(); i++)
	{
		//���X�g���̃t���O��false�̂��̂�T��
		if (!bulletList[i]->GetUsedFlag())
		{
			//false�̃t���O����������Atrue�ɕς���
			bulletList[i]->ChangeUseFlag(true);
			break;
		}
	}
}

void Player::Damage(int damage)
{
	if (!dFlag)
	{
		hp -= damage;                    //�_���[�W�󂯂�
		downTimer->SetTime(noDamageTime);//���G���ԏ�����
		dFlag = true;                    //���G�˓�
	}
}

void Player::Draw()
{
	player->Draw();
	//���X�g�̒���S�������āA�t���O��true�Ȃ̂�T���B
	for (auto test : bulletList)
	{
		//true�̃t���O����������X�V����
		test->Draw();
	}
}

void Player::Move()
{
	//����K�{
	velocity = { 0,0,0 };

	position = player->GetPosition();

	velocity.x += input->GetStick("Vertices") * speed;
	velocity.y += input->GetStick("Horizontal") * speed;

	//float3��+=��operator���Ȃ��B
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;

	//�����蔻����ꏏ�ɓ�����
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);

	player->SetPosition(position);
}

void Player::ChangeDamageFlag(bool flag)
{
	DamageFlag = flag;
}