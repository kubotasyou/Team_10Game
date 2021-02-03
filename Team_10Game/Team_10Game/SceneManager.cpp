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
	fader(new Fader(manager)),//�t�F�[�_�[����
	score(new Score(dxManager))
{
	//�ŏ��̃V�[����ݒ�
	currentScene = (BaseScene*) new Title(this, dxManager, input, sound);

	//�t�F�[�h�̃^�C�v��ݒ�
	fader->SetFaderType(FadeType::Normal);

	//�X�R�A��������
	score->Initialize();
}

SceneManager::~SceneManager()
{
	delete(currentScene);
	delete(fader);
	delete(score);
}

void SceneManager::Initialize()
{
#pragma region �e�N�X�`���ǂݍ���

	// 0�f�o�b�O�e�L�X�g
	Sprite::LoadTexture(0, L"Resources/Texture/debugfont.png");
	// 1�t�F�[�h�p���摜
	Sprite::LoadTexture(1, L"Resources/Texture/black.png");
	// 2�^�C�g���̉摜
	Sprite::LoadTexture(2, L"Resources/Texture/Title.png");
	// 3�G���f�B���O�̉摜
	Sprite::LoadTexture(3, L"Resources/Texture/Ending.png");
	// 4�J�[�\���̉摜
	Sprite::LoadTexture(4, L"Resources/Texture/curssor.png");
	// 5�Q�[���V�[���w�i
	Sprite::LoadTexture(5, L"Resources/Texture/Space.jpg");
	// 6�|�C���^�[�̉摜
	Sprite::LoadTexture(6, L"Resources/Texture/marker.png");
#pragma endregion

#pragma region �T�E���h�ǂݍ���

	sound->LoadWav("3MinutesCooking");
	sound->LoadWav("select");
	sound->LoadWav("clear");
	sound->LoadWav("Title");
	sound->LoadWav("Gameplay");
	sound->LoadWav("Ending");
	sound->LoadWav("EndingDark");

	//MP3�t�@�C���̓ǂݍ���
	//���ʎq(.mp3)���K�v
	//���̎��Ƀt�@�C�������R�s�[���Ă����Ɨǂ�
	sound->LoadMP3("BGM01.mp3");

#pragma endregion

	//�V�[���̏�����(�ǂݍ��݂͂����聪�ɏ���)
	currentScene->Initialize();

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
			currentScene = (BaseScene*) new GamePlay(this, dxManager, input,score,sound);
			break;
		case SceneEnding:
			currentScene = (BaseScene*) new Ending(this, dxManager, input,sound,score);
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
	fader->SetFadeIn(0.5f);  //�t�F�[�h�̎��Ԏw��
	fader->SwitchFade(false);//�t�F�[�h�C��
	this->nextScene = nextScene;
}
