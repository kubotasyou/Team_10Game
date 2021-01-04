#include "Player.h"

#include "SafeDelete.h"

#include <DirectXMath.h>

Player::Player(DirectXManager * dxManager, Input * input)
{
	this->dxManager = dxManager;
	this->input = input;

	//�v���C���[����
	sphereModel = new Model(this->dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");   //���f������
	player = GameObject::Create();         //�I�u�W�F�N�g����
	player->SetModel(sphereModel);         //���f���Z�b�g
	player->SetPosition(XMFLOAT3(0, 5, 0));//�ʒu������

}

Player::~Player()
{
	safe_delete(player);
	safe_delete(sphereModel);
}

void Player::Initialize()
{
	velocity = { 0,0,0 };

	//�����������Ă���
	for (int i = 0; i < 10; i++)
	{
		bl.emplace_back(new Bullet(dxManager, player->GetPosition()));
	}
	
}

void Player::Update()
{
	player->Update();

	Move();

	//�{�^������������g�p�����
	if (input->GetKeyTrigger(KeyCode::Z))
	{
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

		for (int i = 0; i < bl.size(); i++)
		{
			//���X�g���̃t���O��false�̂��̂�T��
			if (!bl[i]->GetUsedFlag())
			{
				//false�̃t���O����������Atrue�ɕς���
				bl[i]->ChangeUsed(true);

				break;
			}
		}
	}

	//���X�g�̒���S�������āA�t���O��true�Ȃ̂�T���B
	for (auto test : bl)
	{
		//ture�̃t���O����������X�V����
		test->Update();
	}
}

void Player::Draw()
{
	player->Draw();

	//���X�g�̒���S�������āA�t���O��true�Ȃ̂�T���B
	for (auto test : bl)
	{
		//ture�̃t���O����������X�V����
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

	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);

	player->SetPosition(position);
}
