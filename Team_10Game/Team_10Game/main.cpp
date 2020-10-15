#include "WinApp.h"
#include "DirectXManager.h"
#include "Input.h"
#include "SafeDelete.h"

#include "Sprite.h"

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


#pragma region テクスチャ初期化

	Sprite::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height);

	Sprite* sprite = nullptr;


	//テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/Texture/kirby.png");

	//オブジェクト生成
	sprite = Sprite::Create(1, { -1.0f,0.0f });

#pragma endregion


	//メインループ
	while (true)
	{
		if (winApp->LoopMessage())
		{
			break;
		}

		input->Update();

		//描画開始↓
		dxManager->BeginDraw();


		if (input->GetKeyDown(KeyCode::SPACE))
		{

		}

		Sprite::BeginDraw(dxManager->GetcmdList());
		sprite->Draw();
		Sprite::EndDraw();

		//描画終了↑
		dxManager->EndDraw();
	}

	//解放
	safe_delete(dxManager);
	safe_delete(input);
	safe_delete(sprite);

	//ゲームウィンドウの破棄
	winApp->ReleaseGameWindow();
	safe_delete(winApp);

	return 0;
}