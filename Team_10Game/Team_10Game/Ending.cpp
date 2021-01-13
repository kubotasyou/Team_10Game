#include "Ending.h"
#include "SafeDelete.h"

Ending::Ending(ISceneChanger* sceneChanger, DirectXManager* dxcommon, Input* input, Sound* sound, Score* score)
	:BaseScene(sceneChanger),//read ベースシーンをここで作っているのかな...
	dxManager(dxcommon),
	input(input),
	sound(sound),
	score(score)
{
}

Ending::~Ending()
{
	safedelete(spriteBG);
	safedelete(curssor);
}

void Ending::Initialize()
{
	// 背景スプライト生成
	spriteBG = Sprite::Create(3, { 0.0f,0.0f });
	//カーソル画像生成
	curssor = Sprite::Create(4, { 0.0f,0.0f });
	//sound->PlayLoopBGM("testBgm.mp3");
	sound->PlayLoop("EndingDark");
}

void Ending::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		NextScene();
	}

	//sound->CheckLoop("testBgm.mp3");
}

void Ending::Draw()
{
	Sprite::BeginDraw(dxManager->GetcmdList());
	// 背景スプライト描画
	spriteBG->Draw();
	score->Draw(300, 350, 8, { 1,0,0,1 });

	switch (selectCount)
	{
	case 0:
		curssor->Draw();
		curssor->SetPosition({ 50.0f,50.0f });
	}

	// スプライト描画後処理
	Sprite::EndDraw();
}

void Ending::NextScene()
{
	sound->Stop();
	//シーン変更(変更したいシーンを入れてね)
	sceneChanger->ChangeScene(SceneTitle);
}
