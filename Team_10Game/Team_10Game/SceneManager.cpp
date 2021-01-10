#include "SceneManager.h"
#include "SafeDelete.h"
#include "Title.h"
#include "Ending.h"
#include "GamePlay.h"

#include "Sprite.h"

SceneManager::SceneManager(DirectXManager * manager, Input * input, Sound* sound)
	:nextScene(SceneNone),    //�ŏ��͉�������Ȃ��ł���
	dxManager(manager),
	input(input),
	sound(sound),
	fader(new Fader(manager))//�t�F�[�_�[����
{
	//�ŏ��̃V�[����ݒ�
	currentScene = (BaseScene*) new GamePlay(this, dxManager, input);
	fader->SetFaderType(FadeType::Normal);
}

SceneManager::~SceneManager()
{
	delete(currentScene);
	delete(fader);
}

void SceneManager::Initialize()
{
	currentScene->Initialize();

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/Texture/debugfont.png");

	//Sprite::LoadTexture()
}

void SceneManager::Update()
{
	fader->Update();

	//�t�F�[�h���������珈�������Ȃ�
	if (fader->GetisFade()) return;

	//���̃V�[����None����Ȃ������珈��
	if (nextScene != SceneNone)
	{
		//���݂̃V�[���I��
		delete currentScene;

		switch (nextScene)
		{
		case SceneTitle:
			//Title�̑�������ISceneChanger;
			//SceneManager��ISceneChanger���p��������̂Ȃ̂�
			//this�����邱�ƂŁATitle�V�[����SceneManager���g����!
			currentScene = (BaseScene*) new Title(this, dxManager, input,sound);
			break;
		case SceneGame:
			currentScene = (BaseScene*) new GamePlay(this, dxManager, input);
			break;
		case SceneEnding:
			currentScene = (BaseScene*) new Ending(this, dxManager, input,sound);
			break;

		default:
			break;
		}

		//�ʂ̃V�[�����Ă񂾂�None�V�[���ŏ�����
		nextScene = SceneNone;
		currentScene->Initialize();
		//��b�����ăt�F�[�h�A�E�g
		fader->SetFadeOut(0.5f);
		fader->SwitchFade(true);
	}

	//�t�F�[�h���������珈�������Ȃ�
	if (fader->GetisFade()) return;

	currentScene->Update();
}

void SceneManager::Draw()
{
	currentScene->Draw();
	fader->Draw();
}

void SceneManager::ChangeScene(SceneType nextScene)
{
	fader->SetFadeIn(0.5f);
	fader->SwitchFade(false);
	this->nextScene = nextScene;
}
