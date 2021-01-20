#pragma once
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Model.h"
#include "CountDownTimer.h"
#include "Sound.h"
#include "BaseScene.h"
#include "Player.h"
#include "Enemy.h"
#include "DebugText.h"
#include "Score.h"
#include "HpText.h"

#include "ParticleManager.h"

class GamePlay :public BaseScene
{
public:
	//�R���X�g���N�^
	GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input, Score* score,Sound* sound);

	//�f�X�g���N�^
	~GamePlay();

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//���̃V�[����
	void NextScene();

private:

	DirectXManager* dxManager;
	Input* input;
	Score* score;

	Sprite* backGround;//�w�i�摜

	Model* spaceDomeModel;//�F���h�[�����f��
	Model* sphereModel; //�����f��
	//Model* groundModel; //�O���E���h���f��
	Model* charaModel;//�v���C���[�p�̃e�X�g���f��

	GameObject* spaceDome;//�F���h�[���I�u�W�F�N�g
	//GameObject* ground;	//�O���E���h�I�u�W�F�N�g

	Player* player;
	std::vector<Enemy*> enemys;
	int enemyNum = 10;//��x�ɕ\������G�̐��B
	bool hit = false;//���i�q�b�g���Ȃ��悤�ɏ����ǉ�
	bool timer = false;//�ǉ�
	int t = 0;//�ǉ�
	int d = 1;//�ǉ�
	DebugText debugText;
	HpText hpText;

	ParticleManager* particleMan = nullptr;
	DirectX::XMFLOAT3 deadPos;
	Sound* sound =nullptr;
};



