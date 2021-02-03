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
	objtest = GameObject::Create();
	objtest->SetModel(sphereModel);
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
	safedelete(objtest);
}

void Player::Initialize()
{
	//�ꉞ������
	velocity = { 0,0,0 };
	pointervelocity = { 0,0,0 };
	//�ʒu������
	position = float3(0, 0, 0);
	rotation = float3(0, 0, 0);
	pointerPosition = float3(0, 0, 15);
	player->SetPosition(position);
	player->SetRotation(rotation);
	player->SetColor({ 1, 0, 0, 0.5f });//��ŏ���
	//���@�F�����蔻�菉����
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);//�ʒu
	sphere.radius = 1.0f;//���a

	objtest->SetPosition(pointerPosition);

	//�̗͏�����
	hp = 5;

	//���G���Ԃ̏�����
	downTimer = new CountDownTimer();
	downTimer->SetTime(noDamageTime);

	//�J�����̈ʒu������
	cameraPosition = float3(position.x, position.y, -5);
	camera->SetTarget(position);//�^�[�Q�b�g�̓v���C���[�̈ʒu
	camera->SetEye(cameraPosition);//�v���C���[�̐^��납�猩�Ă銴��(z��0�ɂ��邱�Ƃ͂ł��Ȃ�)
	bulletTimer = new CountDownTimer();
	bulletTimer->SetTime(bulletTime);
	rotSpeed = 0.4f;
}

void Player::Update()
{
	player->Update();
	objtest->Update();
	bulletTimer->Update();
	Move();
	//CameraMove();
	Blinking();

	//�{�^������������g�p�����
	if (input->GetKeyDown(KeyCode::SPACE))
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

	pointerPosition = objtest->GetPosition();
}
void Player::Shot()
{
	if (bulletTimer->IsTime())
	{
		bulletList.emplace_back(new Bullet(player->GetPosition(), sphereModel,pointerPosition));
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
		bulletTimer->SetTime(bulletTime);//�e���ԏ�����
	}
}

void Player::Blinking()
{
	//�_���[�W���󂯂Ă��Ȃ������珈�����Ȃ�
	if (!damageFlag) return;

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
		player->SetColor({ 1, 1, 1, 0.5f });
	}

	//���ԂɂȂ�����
	if (downTimer->IsTime())
	{
		player->SetColor({ 1, 1, 1, 0.5f });
		//�_���[�W���󂯂Ă��Ȃ���Ԃɂ���
		count = 0;
		damageFlag = false;
	}
}

void Player::Damage(int damage)
{
	if (!damageFlag)
	{
		hp -= damage;                    //�_���[�W�󂯂�
		downTimer->SetTime(noDamageTime);//���G���ԏ�����
		damageFlag = true;               //���G�˓�
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
	//pointer->Draw();
	objtest->Draw();
}

void Player::Move()
{
	//����K�{
	velocity = { 0,0,0 };
	pointervelocity = { 0,0,0 };
	position = player->GetPosition();

	velocity.x = input->GetStick("Vertices") * speed;
	velocity.y = input->GetStick("Horizontal") * speed;
	pointervelocity.x = input->GetRightStick("Vertices") * speed;
	pointervelocity.y = input->GetRightStick("Horizontal") * speed;

	//float3��+=��operator���Ȃ��B
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;
	pointerPosition.x += pointervelocity.x;
	pointerPosition.y += pointervelocity.y;
	pointerPosition.z += pointervelocity.z;

	//�ړ��͈͂̐�����������
	position.x = Clamp(position.x, -5.0f, 5.0f);
	position.y = Clamp(position.y, -3.5f, 2.0f);
	position.z = Clamp(position.z, -0.1f, 0.1f);

	//��]������������
	rotation.x = Clamp(rotation.x, -45.0f, 45.0f);
	rotation.y = Clamp(rotation.y, -45.0f, 45.0f);
	rotation.z = Clamp(rotation.z, -45.0f, 45.0f);
	//�����蔻����ꏏ�ɓ�����
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);

	if (input->GetStick("Vertices")== 1)
	{
		rotation.z = rotation.z - rotSpeed;
	}
	if (input->GetStick("Vertices") == -1)
	{
		rotation.z = rotation.z + rotSpeed;
	}
	if (input->GetStick("Vertices") == 0)
	{
		if (rotation.z >= 0)
		{
			rotation.z =rotation.z - rotSpeed;
		}
		if (rotation.z <= 0)
		{
			rotation.z = rotation.z + rotSpeed;
		}
	}
	if (input->GetStick("Horizontal") == 1)
	{
		rotation.x = rotation.x + rotSpeed;
	}
	if (input->GetStick("Horizontal") == -1)
	{
		rotation.x = rotation.x - rotSpeed;
	}
	if (input->GetStick("Horizontal") == 0)
	{
		if (rotation.x >= 0)
		{
			rotation.x = rotation.x - rotSpeed;
		}
		if (rotation.x <= 0)
		{
			rotation.x = rotation.x + rotSpeed;
		}
	}

	if (velocity.y == 0)
	{

	}
	//�ʒu�X�V
	player->SetPosition(position);
	player->SetRotation(rotation);
	objtest->SetPosition(pointerPosition);
}

void Player::CameraMove()
{
	cameraVelocity = { 0,0,0 };

	cameraVelocity.x = input->GetStick("Vertices") *   speed;
	cameraVelocity.y = input->GetStick("Horizontal") * speed;

	//float3��+=��operator���Ȃ��B
	cameraPosition.x += cameraVelocity.x;
	cameraPosition.y += cameraVelocity.y;

	//�ړ��͈͂̐�����������
	cameraPosition.x = Clamp(cameraPosition.x, -15.0f, 15.0f);
	cameraPosition.y = Clamp(cameraPosition.y, -15.0f, 15.0f);

	//�J�����𓮂���
	camera->SetTarget(position);
	camera->SetEye(cameraPosition);
}

float Player::Clamp(float value, float min, float max)
{
	if (value <= min)
	{
		value = min;
	}
	else if (value >= max)
	{
		value = max;
	}

	return value;

}