#include "GamePlay.h"
#include "SafeDelete.h"
#include "Collision.h"
#include "Random.h"

//�����`��p
#include <sstream>
#include <iomanip>

GamePlay::GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input, Score* score,Sound* sound)
	:BaseScene(sceneChanger),
	dxManager(manager),
	input(input),
	score(score),
	sound(sound)
{

#pragma region ���f���̓ǂݍ��݂ƍ쐬

	//�X�J�C�h�[�����f��
	/*skyDomeModel = new Model(dxManager->GetDevice());
	skyDomeModel->CreateModel("skydome");*/
	//�����f��
	sphereModel = new Model(dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");
	////�O���E���h���f��
	//groundModel = new Model(dxManager->GetDevice());
	//groundModel->CreateModel("ground");

#pragma endregion

	debugText.Initialize(0);
	hpText.Initialie(0);
}


GamePlay::~GamePlay()
{
	//safedelete(skyDomeModel);
	safedelete(sphereModel);
	//safedelete(groundModel);
	safedelete(ground);
	//safedelete(skyDome);
	for(auto e:enemys)
	{
		safedelete(e);
	}
	safedelete(player);
	safedelete(particleMan);
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
	//skyDome = GameObject::Create();
	//skyDome->SetModel(skyDomeModel);

	////�O���E���h
	//ground = GameObject::Create();
	//ground->SetModel(groundModel);
	//ground->SetPosition({ 0, -2, 0 });

	// �p�[�e�B�N���}�l�[�W���[����
	particleMan = ParticleManager::Create();
	sound->PlayLoop("Gameplay");
#pragma endregion

}

void GamePlay::Update()
{
	score->Update();

	player->Update();

	for (auto& e : enemys)
	{
		e->Update(player->GetPosition());
	}

	//skyDome->Update();
	//ground->Update();

	//�̗͂��Ȃ��Ȃ����玟�̃V�[����
	if (player->GetHp() < 1)
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
				//�X�R�A��ǉ�
				score->AddScore(20);
				deadPos = enemys[j]->GetPosition();
			}
		}
	}

	//�G�ƃv���C���[�̓����蔻��
	for (int j = 0; j < enemys.size(); j++)
	{
		bool PtoEs = Collision::SphereToSphere(player->GetSphere(), enemys[j]->GetSphere());
		if (PtoEs)
		{
			enemys[j]->ChangeDeadFlag(true);
			player->Damage(1);
			deadPos = enemys[j]->GetPosition();
		}
	}
#pragma endregion

	//�c�莞�Ԃ�\��
	std::ostringstream timerstr;
	timerstr.clear();
	///////////
	//timerstr << "Time:" << std::fixed << std::setprecision(1) << "Test";
	//debugText.Print(timerstr.str(), 800, 0, 5.0f);
	//////////
	//std::ostringstream hpstr;
	//hpstr.clear();
	//hpstr << "HP:" << std::fixed << std::setprecision(1) << hp;
	//hpText.Print(hpstr.str(), 200, 0, 5.0f);


	std::ostringstream hpstr;
	hpstr.clear();
	hpstr << "HP:" << std::fixed << std::setprecision(1) << player->GetHp();
	hpText.Print(hpstr.str(), 200, 0, 2.0f);

	for (auto& e : enemys)
	{
		if (e->GetDeadFlag())
		{
			for (int i = 0; i < 100; i++)
			{
				//X,Y,Z�S�Ă̍��W��{-0.05f,+0.05f}�Ń����_���ɕ��z
				const float rnd_vel = 0.15f;
				XMFLOAT3 vel{};
				vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
				vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
				vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
				//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
				XMFLOAT3 acc{};
				const float rnd_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * rnd_acc;

				XMFLOAT4 color = { 1.0f, 0.0f, 0.0f, 0.0f };
				float t = 2;

				deadPos = { deadPos.x, deadPos.y + t, deadPos.z };

				//�ǉ�
				particleMan->Add(120, deadPos, vel, acc, 1.0f, 0.0f, color);
			}
		}
	}
	particleMan->Update();
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

	//skyDome->Draw();
	//ground->Draw();

	// 3D�I�u�W�F�N�g�`��㏈��
	GameObject::EndDraw();

	//2D�e�N�X�`���`��O����
	Sprite::BeginDraw(cmdList);

	score->Draw(400, 0, 8);
	debugText.DrawAll(cmdList);
	hpText.DrawAll(cmdList);

	//2D�e�N�X�`���`��㏈��
	Sprite::EndDraw();

	//�p�[�e�B�N���`��
	ParticleManager::PreDraw(cmdList);

	particleMan->Draw();

	ParticleManager::PostDraw();
}

void GamePlay::NextScene()
{
	sceneChanger->ChangeScene(SceneEnding);
}
