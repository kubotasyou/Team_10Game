#include "Title.h"
#include "SafeDelete.h"

Title::Title(ISceneChanger* sceneChanger, DirectXManager * manager, Input * input, Sound* sound)
	:BaseScene(sceneChanger),//read ベースシーンをここで作っているのかな...
	dxManager(manager),
	input(input),
	sound(sound)
{
	
}

Title::~Title()
{
	safe_delete(spriteBG);
}

void Title::Initialize()
{
	// テクスチャ読み込み(テストで５番)
	Sprite::LoadTexture(5, L"Resources/Texture/tex1.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(5, { 0.0f,0.0f });
	sound->Play("3MinutesCooking");
}

void Title::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		
		NextScene();
	}
}

void Title::Draw()
{
	//ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();
	//Sprite::BeginDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	// スプライト描画後処理
	//Sprite::EndDraw();
}

void Title::NextScene()
{
	//シーン変更(変更したいシーンを入れてね)
	sceneChanger->ChangeScene(SceneEnding);
}

