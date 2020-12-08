#include "GamePlay.h"
#include "SafeDelete.h"

#include "Collision.h"


#pragma comment(lib, "xaudio2.lib")

#include <sstream>
#include <iomanip>

GamePlay::GamePlay()
{

}

GamePlay::~GamePlay()
{
	safe_delete(sprite);
	safe_delete(mod);
	safe_delete(obj);
	safe_delete(charaModel);
	safe_delete(chara);
	safe_delete(charaModel2);
	safe_delete(chara2);
}

void GamePlay::Initialize(DirectXManager * dxManager, Input * input)
{
	this->dxManager = dxManager;
	this->input = input;

	////�e�N�X�`���ǂݍ���
	//Sprite::LoadTexture(2, L"Resources/Texture/kirby.png");
	////�I�u�W�F�N�g����
	//sprite = Sprite::Create(2, { -1.0f,0.0f });


	//readme : 1�T�E���h�N���X���g���₷���悤�ɂ���B
	//readme : ���C�̓����蔻��͎g���������������Ă����B
	//readme : 4���_�I�u�W�F�N�g�̕`�������
	//readme : �J�����𕪗�������B
	//readme : �V�[���J�ڂ����Ƃ������
	//readme : �v���C���[���L�����N�^�[�Ǘ��N���X���G�l�~�[



	mod = new Model(dxManager->GetDevice());
	mod->CreateModel("ground");
	obj = GameObject::Create();
	obj->SetModel(mod);

	gNormal = XMFLOAT3(0, 1, 0);


	charaModel = new Model(dxManager->GetDevice());
	charaModel->CreateModel("sphere2");
	chara = GameObject::Create();
	chara->SetModel(charaModel);
	chara->SetPosition(XMFLOAT3(0, 5, 0));

	charaModel2 = new Model(dxManager->GetDevice());
	charaModel2->CreateModel("sphere2");
	chara2 = GameObject::Create();
	chara2->SetModel(charaModel);
	chara2->SetPosition(XMFLOAT3(2.f, 5, 0));

	downTimer = new CountDownTimer();
	downTimer->SetTime(5.0f);

	sound = new Sound();
	sound->LoadSound("Alarm01");
	//sound->Play("Alarm01",0.1f);

	//sound->LoadSound("3MinutesCooking");
	//sound->Play("3MinutesCooking",0.1f);

	sound->LoadSound("GodisSaying");
	//sound->PlayLoop("GodisSaying", 0.1f);
}

void GamePlay::Update()
{
#pragma region �J�����̈ړ�

	if (input->GetKeyDown(KeyCode::A))
	{
		GameObject::CameraMoveVector(XMFLOAT3(-0.1f, 0.0f, 0.0f));
	}
	if (input->GetKeyDown(KeyCode::D))
	{
		GameObject::CameraMoveVector(XMFLOAT3(+0.1f, 0.0f, 0.0f));
	}
	if (input->GetKeyDown(KeyCode::W))
	{
		GameObject::CameraMoveVector(XMFLOAT3(0.0f, 0.0f, 0.1f));
	}
	if (input->GetKeyDown(KeyCode::S))
	{
		GameObject::CameraMoveVector(XMFLOAT3(0.0f, 0.0f, -0.1f));
	}

#pragma endregion

#pragma region �I�u�W�F�N�g�̈ړ�

	if (input->GetKeyDown(KeyCode::RIGHT) ||
		input->GetKeyDown(KeyCode::LEFT) ||
		input->GetKeyDown(KeyCode::UP) ||
		input->GetKeyDown(KeyCode::DOWN))
	{
		XMFLOAT3 position = chara->GetPosition();

		if (input->GetKeyDown(KeyCode::RIGHT))
		{
			position.x += 0.05f;
		}
		else if (input->GetKeyDown(KeyCode::LEFT))
		{
			position.x -= 0.05f;
		}
		else if (input->GetKeyDown(KeyCode::UP))
		{
			position.y += 0.05f;
		}
		else if (input->GetKeyDown(KeyCode::DOWN))
		{
			position.y -= 0.05f;

		}

		chara->SetPosition(position);
	}

#pragma endregion

	//ReadMe : �v���C���[�ɃJ������Ǐ]��������

#pragma region ���ƒn��

	//���ƒn��
	bool hit = Collision::CheckSphereToPanel(chara->GetPosition(), chara->GetRadius(), gNormal, 0.0f);

	if (hit)
	{
		//ReadMe : �����ɓ�����������d����������
		chara->SetColor(XMFLOAT4(1, 0, 0, 1));
	}

#pragma endregion

#pragma region ���Ƌ��̔���

	//���Ƌ�
	bool hit2 = Collision::CircleToCircle(chara->GetPosition(), chara->GetRadius(), chara2->GetPosition(), chara2->GetRadius());

	if (hit2)
	{
		//ReadMe : �����ɓ�����������d����������
		chara->SetColor(XMFLOAT4(1, 0, 0, 1));
	}

#pragma endregion


	if (!hit && !hit2)
	{
		chara->SetColor(XMFLOAT4(1, 1, 1, 1));
	}

	if (input->GetKeyTrigger(KeyCode::Z))
	{
		sound->Play("GodisSaying", 0.1f);
	}
	if (input->GetJoyPadRelease(JoyPad::A))
	{
		sound->Play("Alarm01", 0.1f);
	}

#pragma region ���ԂɂȂ�����n

	//���ԂɂȂ����烂�f���`�F���W
	if (downTimer->IsTime())
	{
		//sound->Play("Alarm01");
	}

#pragma endregion

	chara->SetTarget(chara->GetPosition());

	obj->Update();
	chara->Update();
	chara2->Update();
	downTimer->Update();
}

void GamePlay::Draw()
{
	Sprite::BeginDraw(dxManager->GetcmdList());
	//sprite->Draw();
	Sprite::EndDraw();

	GameObject::BeginDraw(dxManager->GetcmdList());
	obj->Draw();
	chara->Draw();
	chara2->Draw();
	GameObject::EndDraw();
}