#include "WinApp.h"
#include "DirectXManager.h"
#include "Input.h"
#include "SafeDelete.h"
#include "SceneManager.h"
#include "GamePlay.h"
#include "Sound.h"
#include "ParticleManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* winApp = nullptr;
	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;

	//�E�B���h�E�A�v������
	winApp = new WinApp();
	winApp->CreateGameWindow();

	//DirectX������
	dxManager = new DirectXManager();
	dxManager->Initialize(winApp);

	//Input������
	input = new Input();
	input->Initialize(winApp->GetInstance(), winApp->GetHwnd());

	//������
	Sprite::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height);
	GameObject::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height);

	// �p�[�e�B�N���}�l�[�W���[�ÓI������
	if (!ParticleManager::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height)) 
	{
		assert(0);
		return 1;
	}

	//�T�E���h
	Sound* sound = new Sound();

	//�V�[���}�l�[�W���[
	SceneManager* sceneManager = new SceneManager(dxManager, input, sound);
	sceneManager->Initialize();


	//���C�����[�v
	while (true)
	{
		if (winApp->LoopMessage())
		{
			break;
		}

		//���͍X�V
		input->Update();

		//�V�[���}�l�[�W���[�X�V
		sceneManager->Update();

		//�`��J�n��
		dxManager->BeginDraw();

		//�V���}�l�`��
		sceneManager->Draw();

		//�`��I����
		dxManager->EndDraw();
	}

	//���
	safedelete(dxManager);
	safedelete(input);
	safedelete(sceneManager);
	//safedelete(sound);

	//�Q�[���E�B���h�E�̔j��
	winApp->ReleaseGameWindow();
	safedelete(winApp);

	return 0;
}