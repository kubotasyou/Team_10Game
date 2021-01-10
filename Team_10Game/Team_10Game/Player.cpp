#include "Player.h"

#include "SafeDelete.h"

#include <DirectXMath.h>

Player::Player(DirectXManager * dxManager, Input * input)
{
	this->dxManager = dxManager;
	this->input = input;

	//�v���C���[����
	sphereModel = new Model(this->dxManager->GetDevice());
<<<<<<< HEAD
	sphereModel->CreateModel("sphere2");
	player = GameObject::Create();
	player->SetModel(sphereModel);
	player->SetPosition(XMFLOAT3(0, 5, 0));

	//�����̋��̐���
	collider = GameObject::Create();
	collider->SetModel(sphereModel);
	//�����ƂȂ�ʒu�ɐݒu!
	collider->SetPosition(XMFLOAT3(player->GetPosition().x, player->GetPosition().y - player->GetRadius(), player->GetPosition().z));
	//�T�C�Y�͏�����
	collider->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	//�F�͐ԐF
	collider->SetColor({ 1,0,0,1 });
	//�����̋��̔��a��ύX
	collider->SetRadius(colliderRadius);

	sound = new Sound();
	sound->LoadSE("Alarm01");

	timer = new CountDownTimer();
	timer->SetTime(1.5f);
=======
	sphereModel->CreateModel("sphere2");   //���f������
	player = GameObject::Create();         //�I�u�W�F�N�g����
	player->SetModel(sphereModel);         //���f���Z�b�g
	player->SetPosition(XMFLOAT3(0, 5, 0));//�ʒu������

>>>>>>> Masataka
}

Player::~Player()
{
	safe_delete(player);
	safe_delete(sphereModel);
	//std::vector<Bullet*>::iterator it = bl.begin();
	//for (; it != bl.end(); ++it) {
	//	delete *it;
	//}
}

void Player::Initialize()
{
	velocity = { 0,0,0 };

	//�����������Ă���
	//for (int i = 0; i < 10; i++)
	//{
	//	bl.emplace_back(new Bullet(dxManager, player->GetPosition()));
	//}

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
		Shot();

	}
	//���X�g�̒���S�������āA�t���O��true�Ȃ̂�T���B
	for (auto test : bl)
	{
		//ture�̃t���O����������X�V����
		test->Update();
	}

}
void Player::Shot()
{
	bl.emplace_back(new Bullet(player->GetPosition(), sphereModel));
	for (int i = 0; i < bl.size(); i++)
	{
		////���X�g���̃t���O��false�̂��̂�T��
		if (!bl[i]->GetUsedFlag())
		{
			//false�̃t���O����������Atrue�ɕς���
			bl[i]->ChangeUsed(true);
			break;
		}
	}

}

void Player::BulletDel()
{
}


void Player::Draw()
{
	player->Draw();

	//���X�g�̒���S�������āA�t���O��true�Ȃ̂�T���B
	for (auto test : bl)
	{
		//true�̃t���O����������X�V����
		test->Draw();
	}
}

void Player::Move()
{
<<<<<<< HEAD
	if (input->GetJoyPadRelease(JoyPad::B))
	{
		sound->PlaySE("Alarm01", 0.2f);
	}
=======
	//����K�{
	velocity = { 0,0,0 };

	position = player->GetPosition();

	velocity.x += input->GetStick("Vertices") * speed;
	velocity.y += input->GetStick("Horizontal") * speed;

	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);

	player->SetPosition(position);
>>>>>>> Masataka
}
