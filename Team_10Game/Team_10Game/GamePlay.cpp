#include "GamePlay.h"
#include "SafeDelete.h"

#include "Collision.h"


#pragma comment(lib, "xaudio2.lib")

#include <sstream>
#include <iomanip>

GamePlay::GamePlay(ISceneChanger* sceneChanger,DirectXManager* manager,Input* input)
	:BaseScene(sceneChanger),
	dxManager(manager),
	input(input)
{

}
const int ObjectSize = 10;
Object3D Transform[ObjectSize];
float posx;
float posy;
float posz;
GamePlay::~GamePlay()
{
	//safe_delete(sprite);
	safe_delete(groundModel);
	safe_delete(ground);
	safe_delete(charaModel);
	safe_delete(chara);
	safe_delete(charaModel2);
	safe_delete(chara2);
	safe_delete(bullet[ObjectSize]);
	safe_delete(player);
}

void GamePlay::Initialize()
{
	
	////�e�N�X�`���ǂݍ���
	//Sprite::LoadTexture(2, L"Resources/Texture/kirby.png");
	////�I�u�W�F�N�g����
	//sprite = Sprite::Create(2, { -1.0f,0.0f });


	//readme : 1�T�E���h�N���X���g���₷���悤�ɂ���B
	//readme : ���C�̓����蔻��͎g���������������Ă����B
	//readme : �J�����𕪗�������B
	//readme : �V�[���J�ڂ����Ƃ������
	//readme : �v���C���[���L�����N�^�[�Ǘ��N���X���G�l�~�[



	groundModel = new Model(dxManager->GetDevice());
	groundModel->CreateModel("ground");
	ground = GameObject::Create();
	ground->SetModel(groundModel);

	gNormal = XMFLOAT3(0, 1, 0);


	charaModel = new Model(dxManager->GetDevice());
	charaModel->CreateModel("sphere2");
	chara = GameObject::Create();
	chara->SetModel(charaModel);
	chara->SetPosition(XMFLOAT3(0, 5, 0));

	charaModel2 = new Model(dxManager->GetDevice());
	charaModel2->CreateModel("sphere2");
	chara2 = GameObject::Create();
	chara2->SetModel(charaModel2);
	chara2->SetPosition(XMFLOAT3(2.f, 5, 0));

	charaModel3 = new Model(dxManager->GetDevice());
	charaModel3->CreateModel("sphere2");
	for (int i = 0; i < ObjectSize; i++)
	{
		bullet[i] = GameObject::Create();
		bullet[i]->SetModel(charaModel3);
		posz = 1000;
	}
	downTimer = new CountDownTimer();
	downTimer->SetTime(5.0f);

	sound = new Sound();
	sound->LoadSound("Alarm01");
	//sound->Play("Alarm01",0.1f);

	//sound->LoadSound("3MinutesCooking");
	//sound->Play("3MinutesCooking",0.1f);

	sound->LoadSound("GodisSaying");
	//sound->PlayLoop("GodisSaying", 0.1f);


	player = new Player(dxManager, input);
	player->Initialize();
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
	XMFLOAT3 position = chara->GetPosition();
	if (input->GetKeyDown(KeyCode::RIGHT) ||
		input->GetKeyDown(KeyCode::LEFT) ||
		input->GetKeyDown(KeyCode::UP) ||
		input->GetKeyDown(KeyCode::DOWN))
	{
		

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

#pragma region �e�֘A

	for (int i = 0; i < ObjectSize; i++)
	{
		bullet[i]->SetPosition(XMFLOAT3(posx, posy, posz));
	}
	if (input->GetKeyTrigger(KeyCode::X))
	{

		for (int i = 0; i < ObjectSize; i++)
		{		
			bullet[i] = GameObject::Create();
			posx = position.x;
			posy = position.y;
			posz = position.z;
			bullet[i]->SetModel(charaModel3);	
			break;
		}
	}
	posz++;
#pragma endregion

#pragma region ���ƒn��

	//ReadMe : �v���C���[�ɃJ������Ǐ]��������
	// ���ƒn��
	bool hit = Collision::CheckSphereToPanel(chara->GetPosition(), chara->GetRadius(), gNormal, 0.0f);

	if (hit)
	{
		//ReadMe : �����ɓ�����������d����������
		chara->SetColor(XMFLOAT4(1, 0, 0, 1));
	}
#pragma region �v���C���[�ƒn��
	//�v���C���[�ƒn��
	bool isGround = Collision::CheckSphereToPanel(player->GetColliderPos(), player->GetColliderRadius() , gNormal, 0.0f);

	/*if (isGround)
	{*/
		//ReadMe : �����ɓ����������̏���������

		//�v���C���[�ɓ������������Ԃ�
		player->SetisGround(isGround);
	//}

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
	if (!isGround && !hit2)
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
		/*sound->Play("Alarm01");*/
	}

#pragma endregion

	chara->SetTarget(chara->GetPosition());
	chara->Update();
	chara2->Update();

	for (int i = 0; i < ObjectSize; i++)
	{
		bullet[i]->Update();
	}
	player->Update();
	ground->Update();
	downTimer->Update();
}

void GamePlay::Draw()
{
	//Sprite::BeginDraw(dxManager->GetcmdList());
	//sprite->Draw();
	//Sprite::EndDraw();

	GameObject::BeginDraw(dxManager->GetcmdList());
	chara->Draw();
	GameObject::EndDraw();
	GameObject::BeginDraw(dxManager->GetcmdList());
	chara2->Draw();
	GameObject::EndDraw();
	GameObject::BeginDraw(dxManager->GetcmdList());
	for (int i = 0; i < ObjectSize; i++)
	{
	    bullet[i]->Draw();
    }
	GameObject::EndDraw();
	GameObject::BeginDraw(dxManager->GetcmdList());
	ground->Draw();
	GameObject::EndDraw();
	GameObject::BeginDraw(dxManager->GetcmdList());
	player->Draw();
	GameObject::EndDraw();
}
void GamePlay::NextScene()
{
	sceneChanger->ChangeScene(SceneEnding);
}
