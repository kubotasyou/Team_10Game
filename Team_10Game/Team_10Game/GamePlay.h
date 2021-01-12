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
#include "HpText.h"

class GamePlay :public BaseScene
{
public:
	//�R���X�g���N�^
	GamePlay(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input);

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

	Model* skyDomeModel;//�X�J�C�h�[�����f��
	Model* sphereModel; //�����f��
	Model* groundModel; //�O���E���h���f��

	GameObject* skyDome;//�X�J�C�h�[���I�u�W�F�N�g
	GameObject* ground;	//�O���E���h�I�u�W�F�N�g

	Player* player;
	std::vector<Enemy*> enemys;
	int enemyNum = 10;//��x�ɕ\������G�̐��B

	DebugText debugText;
	HpText hpText;
	//memo : �f�o�b�O�e�L�X�g���g���Ȃ��̂ŁASprite�N���X���m�F����

};



