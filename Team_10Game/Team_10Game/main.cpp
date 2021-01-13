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

	//ウィンドウアプリ生成
	winApp = new WinApp();
	winApp->CreateGameWindow();

	//DirectX初期化
	dxManager = new DirectXManager();
	dxManager->Initialize(winApp);

	//Input初期化
	input = new Input();
	input->Initialize(winApp->GetInstance(), winApp->GetHwnd());

	//初期化
	Sprite::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height);
	GameObject::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height);

	// パーティクルマネージャー静的初期化
	if (!ParticleManager::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height)) 
	{
		assert(0);
		return 1;
	}

	////ゲームシーン初期化
	//GamePlay* gameplay = new GamePlay();
	//gameplay->Initialize(dxManager, input);

	Sound* sound = new Sound();

	SceneManager* sceneManager = new SceneManager(dxManager, input, sound);
	sceneManager->Initialize();


	//メインループ
	while (true)
	{
		if (winApp->LoopMessage())
		{
			break;
		}

		input->Update();

		//gameScene->Update();

		sceneManager->Update();

		//描画開始↓
		dxManager->BeginDraw();

		//gameScene->Draw();
		sceneManager->Draw();

		//描画終了↑
		dxManager->EndDraw();
	}

	//解放
	safedelete(dxManager);
	safedelete(input);
	//safe_delete(gameplay);
	safedelete(sceneManager);

	//ゲームウィンドウの破棄
	winApp->ReleaseGameWindow();
	safedelete(winApp);

	return 0;
}