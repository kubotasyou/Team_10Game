#include "Ending.h"
#include "SafeDelete.h"

Ending::Ending(ISceneChanger * sceneChanger, DirectXManager * manager, Input * input,Sound * sound)
	:BaseScene(sceneChanger),//read ベースシーンをここで作っているのかな...
	dxManager(manager),
	input(input),
	sound(sound)
{
}

Ending::~Ending()
{
	safe_delete(spriteBG);
}

void Ending::Initialize()
{
	// テクスチャ読み込み(テストで５番)
	Sprite::LoadTexture(6, L"Resources/Texture/kirby.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(6, { 0.0f,0.0f });
}

void Ending::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		NextScene();
	}
}

void Ending::Draw()
{
	//ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();
	//Sprite::BeginDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	// スプライト描画後処理
	//Sprite::EndDraw();
}

void Ending::NextScene()
{
	//シーン変更(変更したいシーンを入れてね)
	sceneChanger->ChangeScene(SceneTitle);
}
