#include "GamePlay.h"
#include "SafeDelete.h"
#include "Collision.h"
#include "Random.h"

//�����`��p
#include <sstream>
#include <iomanip>

GamePlay::GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input, Score* score, Sound* sound)
	:BaseScene(sceneChanger),
	dxManager(manager),
	input(input),
	score(score),
	sound(sound)
{

#pragma region ���f���̓ǂݍ��݂ƍ쐬

	//�F�����f��
	spaceDomeModel = new Model(dxManager->GetDevice());
	spaceDomeModel->CreateModel("spacedome");
	//�����f��
	sphereModel = new Model(dxManager->GetDevice());
	sphereModel->CreateModel("sphere2");
	////�O���E���h���f��
	//groundModel = new Model(dxManager->GetDevice());
	//groundModel->CreateModel("ground");

	//�v���C���[�e�X�g�p���f��
	charaModel = new Model(dxManager->GetDevice());
	charaModel->CreateModel("chr_sword");

#pragma endregion

	debugText.Initialize(0);
	hpText.Initialie(0);


#pragma region �����蔻��e�X�g

	rect.x = 0.0f;
	rect.y = 0.0f;

	rect.width = 60.0f;
	rect.height = 60.0f;

	point.x = 50.0f;
	point.x = 50.0f;

	sphere.center = XMVectorSet(0, 0, 0, 1);
	sphere.radius = 1.0f;

#pragma endregion


}


GamePlay::~GamePlay()
{
	//safedelete(skyDomeModel);
	safedelete(sphereModel);
	//safedelete(groundModel);
	//safedelete(ground);
	//safedelete(skyDome);
	for (auto e : enemys)
	{
		safedelete(e);
	}
	safedelete(player);
	safedelete(particleMan);
}

void GamePlay::Initialize()
{

#pragma region �I�u�W�F�N�g�̍쐬

	//�v���C���[
	player = new Player(input, charaModel);
	player->Initialize();

	//�G
	enemys.resize(enemyNum);
	for (int i = 0; i < enemyNum; i++)
	{
		enemys[i] = new Enemy(sphereModel);
		enemys[i]->Initialize(player->GetPosition());
	}

	//�F���h�[��
	spaceDome = GameObject::Create();
	spaceDome->SetModel(spaceDomeModel);
	spaceDome->SetScale({ 0.5f,0.5f,0.5f });

	////�O���E���h
	//ground = GameObject::Create();
	//ground->SetModel(groundModel);
	//ground->SetPosition({ 0, -2, 0 });

	// �p�[�e�B�N���}�l�[�W���[
	particleMan = ParticleManager::Create();
	sound->PlayLoopWav("Gameplay");

	//�w�i�摜
	backGround = Sprite::Create(5, { 0,0 });

#pragma endregion

}

void GamePlay::Update()
{
	score->Update();

	//�v���C���[
	player->Update();

	//�G
	for (auto& e : enemys)
	{
		e->Update(player->GetPosition());
	}

	//�F���h�[��
	spaceDome->Update();

	//�O���E���h
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

	////��������̈ʒu��\��
	//std::ostringstream somethingPos;
	//somethingPos.clear();
	//somethingPos << "CameraPos:" << std::fixed << std::setprecision(1) << player->GetPosition().x << player->GetPosition().y << player->GetPosition().z;
	//debugText.Print(somethingPos.str(), 0, 500, 5.0f);



	std::ostringstream hpstr;
	hpstr.clear();
	hpstr << "HP:" << std::fixed << std::setprecision(1) << player->GetHp();
	hpText.Print(hpstr.str(), 0, 0, 5.0f);

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

				//�ǉ�
				particleMan->Add(60, deadPos, vel, acc, 1.0f, 0.0f, color);
			}
		}
	}
	particleMan->Update();



//#pragma region �����蔻��e�X�g
//
//	/*bool ReToPo = Collision::CheckRectangleToDot(rect, point);*/
//	bool ReToPo = Collision::CheckRectangleToSphere(rect, sphere);
//	if (ReToPo)
//	{
//		std::ostringstream test;
//		test.clear();
//		test << "Rectangle&Dot...Hit!!:" << std::fixed;
//		hpText.Print(test.str(), 0, 400, 5.0f);
//	}
//	else
//	{
//		std::ostringstream test;
//		test.clear();
//		test << "Rectangle&Dot...No!!:" << std::fixed;
//		hpText.Print(test.str(), 0, 400, 5.0f);
//	}
//
//	if (input->GetKeyDown(KeyCode::A))
//	{
//		sphere.center.m128_f32[0] -= 0.1f;
//	}
//	else if (input->GetKeyDown(KeyCode::D))
//	{
//		sphere.center.m128_f32[0] += 0.1f;
//	}
//	if (input->GetKeyDown(KeyCode::S))
//	{
//		sphere.center.m128_f32[1] -= 0.1f;
//	}
//	else if (input->GetKeyDown(KeyCode::W))
//	{
//		sphere.center.m128_f32[1] += 0.1f;
//	}
//	if (input->GetKeyDown(KeyCode::DOWN))
//	{
//		sphere.center.m128_f32[2] -= 0.1f;
//	}
//	else if (input->GetKeyDown(KeyCode::UP))
//	{
//		sphere.center.m128_f32[2] += 0.1f;
//	}
//
//	//��������̈ʒu��\��
//	std::ostringstream somethingPos;
//	somethingPos.clear();
//	somethingPos << "PointPos:" << 
//		std::fixed << 
//		std::setprecision(1) << 
//		sphere.center.m128_f32[0] << "," << 
//		sphere.center.m128_f32[1] << "," <<
//		sphere.center.m128_f32[2];
//
//	debugText.Print(somethingPos.str(), 0, 500, 5.0f);
//
//#pragma endregion

}

void GamePlay::Draw()
{

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();

	// 3D�I�u�W�F�N�g�`��O����
	GameObject::BeginDraw(cmdList);

	//�v���C���[
	player->Draw();

	//�G
	for (auto& e : enemys)
	{
		e->Draw();
	}

	//�F���h�[��
	spaceDome->Draw();

	//�O���E���h
	//ground->Draw();

	// 3D�I�u�W�F�N�g�`��㏈��
	GameObject::EndDraw();



	//�p�[�e�B�N���`��
	ParticleManager::PreDraw(cmdList);

	particleMan->Draw();

	ParticleManager::PostDraw();



	//2D�e�N�X�`���`��O����
	Sprite::BeginDraw(cmdList);

	//backGround->Draw();
	score->Draw(800, 0, 5);
	debugText.DrawAll(cmdList);
	hpText.DrawAll(cmdList);

	//2D�e�N�X�`���`��㏈��
	Sprite::EndDraw();




}

void GamePlay::NextScene()
{
	sound->StopWav();
	sceneChanger->ChangeScene(SceneEnding);
}
