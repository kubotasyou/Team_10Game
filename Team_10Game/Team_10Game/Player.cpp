#include "Player.h"

#include "SafeDelete.h"

#include <DirectXMath.h>

Player::Player(DirectXManager * dxManager, Input * input)
{
	this->dxManager = dxManager;
	this->input = input;

	//�v���C���[����
	sphereModel = new Model(this->dxManager->GetDevice());
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
}

Player::~Player()
{
	safe_delete(player);
	safe_delete(sphereModel);
	safe_delete(collider);
}

void Player::Initialize()
{
	velocity = { 0,0,0 };
}

void Player::Update()
{
	player->Update();
	collider->Update();

	Move();
	Jump();
}

void Player::Draw()
{
	player->Draw();
	collider->Draw();
}

void Player::Move()
{
	//����K�{
	velocity = { 0,0,0 };

	position = player->GetPosition();

	velocity.x += input->GetStick("Vertices") * speed;
	velocity.z += input->GetStick("Horizontal") * speed;

	//�n�ʂɂ��Ă��Ȃ��Ƃ�
	if (!isGround)
	{
		player->SetColor({ 1,0,0,1 });

		//�d��(��)
		velocity.y -= 0.2f;

	}

	if (isJumpFlag)
	{
		timer->Update();

		velocity.y += 2.0f;

		if (timer->IsTime())
		{

		}
	}

	if(isGround)
	{
		//�W�����v���ĂȂ���Ԃɂ���
		isJumpFlag = false;

		//�{�^������������
		if (!isJumpFlag && input->GetJoyPadTrigger(JoyPad::RB))
		{
			//�W�����v���Ă����Ԃɂ���
			isJumpFlag = true;
		}
		player->SetColor({ 0,1,0,1 });
	}

	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);

	player->SetPosition(position);

	/*�t���[������������
	�@��莞�ԏ㏸��������(���̊ԏd�͂͂Ȃ�)
	 �@���ԂɂȂ�����㏸���I�����A�d�͂�t����B*/


	//��Ƀv���C���[�̑����ɐݒu
	collider->SetPosition(XMFLOAT3(player->GetPosition().x, player->GetPosition().y - player->GetRadius(), player->GetPosition().z));
}

void Player::Jump()
{
	if (input->GetJoyPadRelease(JoyPad::B))
	{
		sound->PlaySE("Alarm01", 0.2f);
	}
}
