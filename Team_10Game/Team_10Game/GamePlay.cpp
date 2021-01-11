#include "GamePlay.h"
#include "SafeDelete.h"
#include "Collision.h"

//�����`��p
#include <sstream>
#include <iomanip>

GamePlay::GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input)
	:BaseScene(sceneChanger),
	dxManager(manager),
	input(input)
{

#pragma region ���f���̓ǂݍ��݂ƍ쐬

	//�X�J�C�h�[�����f��
	skyDomeModel = new Model(dxManager->GetDevice());
	skyDomeModel->CreateModel("skydome");
	//�����f��
	sphereModel = new Model(dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");
	//�O���E���h���f��
	groundModel = new Model(dxManager->GetDevice());
	groundModel->CreateModel("ground");

#pragma endregion

	debugText.Initialize(0);

}


GamePlay::~GamePlay()
{
	safedelete(skyDomeModel);
	safedelete(sphereModel);
	safedelete(groundModel);
	safedelete(ground);
	safedelete(skyDome);
	for(auto e:enemys)
	{
		safedelete(e);
	}
	safedelete(player);
}

void GamePlay::Initialize()
{

#pragma region ����������

	//�v���C���[
	player = new Player(input, sphereModel);
	player->Initialize();

	//�G
	enemys.resize(enemyNum);
	for (int i = 0; i < enemyNum; i++)
	{
		enemys[i] = new Enemy(sphereModel);
		enemys[i]->Initialize();
	}

	//�X�J�C�h�[��
	skyDome = GameObject::Create();
	skyDome->SetModel(skyDomeModel);

	//�O���E���h
	ground = GameObject::Create();
	ground->SetModel(groundModel);
	ground->SetPosition({ 0, -2, 0 });

#pragma endregion

}

void GamePlay::Update()
{
	player->Update();

	for (auto& e : enemys)
	{
		e->Update();
	}

	skyDome->Update();
	ground->Update();

	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		NextScene();
	}

#pragma region �����蔻�菈��

	//�e��S����
	for (int i = 0; i < player->GetBulletList().size(); i++)
	{
		//�G��S����
		for (int j = 0; j < enemys.size(); j++)
		{
			bool BstoEs = Collision::SphereToSphere(player->GetBulletList()[i]->GetSphere(), enemys[j]->GetSphere());
			if (BstoEs)
			{
				//�e���폜
				player->GetBulletList()[i]->ChangeDeadFlag(true);
				//�G���폜
				enemys[j]->ChangeDeadFlag(true);
			}
		}
	}

#pragma endregion

	//�c�莞�Ԃ�\��
	std::ostringstream timerstr;
	timerstr.clear();
	timerstr << "Time:" << std::fixed << std::setprecision(1) << "Test";
	debugText.Print(timerstr.str(), 800, 0, 5.0f);

}

void GamePlay::Draw()
{

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();
	// 3D�I�u�W�F�N�g�`��O����
	GameObject::BeginDraw(cmdList);

	player->Draw();
	for (auto& e : enemys)
	{
		e->Draw();
	}

	skyDome->Draw();
	ground->Draw();

	// 3D�I�u�W�F�N�g�`��㏈��
	GameObject::EndDraw();

	Sprite::BeginDraw(cmdList);

	debugText.DrawAll(cmdList);

	Sprite::EndDraw();
}

void GamePlay::NextScene()
{
	sceneChanger->ChangeScene(SceneEnding);
}
