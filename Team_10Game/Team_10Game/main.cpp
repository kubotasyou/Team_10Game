#include "WinApp.h"
#include "DirectXManager.h"
#include "Input.h"
#include "SafeDelete.h"

#include "Sprite.h"
#include "Object3D.h"
#include "GameObject.h"
#include "Model.h"

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

	GameObject* obj1 = nullptr;
	Model* mod1 = new Model(dxManager->GetDevice());


#pragma region テクスチャ初期化

	Sprite::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height);
	/*Object3D::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height);*/
	GameObject::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height);

	Sprite* sprite = nullptr;


	//テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/Texture/kirby.png");

	//オブジェクト生成
	sprite = Sprite::Create(1, { -1.0f,0.0f });

#pragma endregion

#pragma region オブジェクト初期化

	mod1->CreateModel("ground");
	obj1 = GameObject::Create();
	obj1->SetModel(mod1);

	//Object3D* object3d = nullptr;
	//object3d = Object3D::Create();
	//object3d->Update();

#pragma endregion

	//メインループ
	while (true)
	{
		if (winApp->LoopMessage())
		{
			break;
		}

		input->Update();
		//object3d->Update();

		//描画開始↓
		dxManager->BeginDraw();


		if (input->GetKeyDown(KeyCode::SPACE))
		{

		}

		obj1->Update();


		Sprite::BeginDraw(dxManager->GetcmdList());
		sprite->Draw();
		Sprite::EndDraw();

		GameObject::BeginDraw(dxManager->GetcmdList());
		obj1->Draw();
		GameObject::EndDraw();

		//Object3D::BeginDraw(dxManager->GetcmdList());
		//object3d->Draw();
		//Object3D::EndDraw();

		//描画終了↑
		dxManager->EndDraw();
	}

	//解放
	safe_delete(dxManager);
	safe_delete(input);
	safe_delete(sprite);
	safe_delete(mod1);
	safe_delete(obj1);

	//ゲームウィンドウの破棄
	winApp->ReleaseGameWindow();
	safe_delete(winApp);

	return 0;
}