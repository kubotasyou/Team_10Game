#include "WinApp.h"
#include "DirectXManager.h"
#include "Input.h"
#include "SafeDelete.h"
#include "SceneManager.h"
#include "GamePlay.h"

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

	////�Q�[���V�[��������
	//GamePlay* gameplay = new GamePlay();
	//gameplay->Initialize(dxManager, input);


	SceneManager* sceneManager = new SceneManager(dxManager, input);
	sceneManager->Initialize();

	//���C�����[�v
	while (true)
	{
		if (winApp->LoopMessage())
		{
			break;
		}

		input->Update();

		//gameScene->Update();

		sceneManager->Update();

		//�`��J�n��
		dxManager->BeginDraw();

		//gameScene->Draw();
		sceneManager->Draw();

		//�`��I����
		dxManager->EndDraw();
	}

	//���
	safe_delete(dxManager);
	safe_delete(input);
	//safe_delete(gameplay);
	safe_delete(sceneManager);

	//�Q�[���E�B���h�E�̔j��
	winApp->ReleaseGameWindow();
	safe_delete(winApp);

	return 0;
}