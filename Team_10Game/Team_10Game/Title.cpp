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
	safedelete(spriteBG);
}

void Title::Initialize()
{
	// 背景スプライト生成
	spriteBG = Sprite::Create(2, { 0.0f,0.0f });
	//sound->PlaySE("3MinutesCooking",0.1f);
	sound->PlayLoopWav("Title");
}

void Title::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE)||input->GetJoyPadTrigger(JoyPad::A))
	{	
		sound->PlayWav("select", 1.0f);
		NextScene();
	}
}

void Title::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();
	Sprite::BeginDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	// スプライト描画後処理
	Sprite::EndDraw();
}

void Title::NextScene()
{
	sound->StopWav();
	//シーン変更(変更したいシーンを入れてね)
	sceneChanger->ChangeScene(SceneGame);
}

