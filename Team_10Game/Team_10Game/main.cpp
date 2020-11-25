#include "WinApp.h"
#include "DirectXManager.h"
#include "Input.h"
#include "SafeDelete.h"

#include "Title.h"
#include "GamePlay.h"

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

	//ゲームシーン初期化
	GamePlay* gameplay = new GamePlay();
	gameplay->Initialize(dxManager, input);
	Title* title = new Title();
	title->Initialize(dxManager, input);
	bool test = false;

	//メインループ
	while (true)
	{
		if (winApp->LoopMessage())
		{
			break;
		}

		input->Update();

		if (test == true &&input->GetKeyTrigger(KeyCode::SPACE))
		{
			test = false;
		}
		else if (test == false && input->GetKeyTrigger(KeyCode::SPACE))
		{
			test = true;
		}

		if (!test)
		{
			gameplay->Update();
		}
		else
		{

			title->Update();
		}

		//描画開始↓
		dxManager->BeginDraw();

		if (!test)
		{
			gameplay->Draw();
		}
		else
		{
			title->Draw();
		}

		//描画終了↑
		dxManager->EndDraw();
	}

	//解放
	safe_delete(dxManager);
	safe_delete(input);
	safe_delete(gameplay);
	safe_delete(title);

	//ゲームウィンドウの破棄
	winApp->ReleaseGameWindow();
	safe_delete(winApp);

	return 0;
}