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
		curssor->SetPosition({ 50.0f,490.0f });
		if (input->GetKeyTrigger(KeyCode::RIGHT))
		{
			selectCount = 1;
		}
		if (input->GetKeyTrigger(KeyCode::LEFT))
		{
			selectCount = 1;
		}
		break;
	case 1:
		curssor->Draw();
		curssor->SetPosition({ 800.0f,490.0f });
		if (input->GetKeyTrigger(KeyCode::RIGHT))
		{
			selectCount = 0;
		}
		if (input->GetKeyTrigger(KeyCode::LEFT))
		{
			selectCount = 0;
		}
		break;
	}

	// スプライト描画後処理
	Sprite::EndDraw();
}

void Ending::NextScene()
{
	sound->Stop();
	//シーン変更(変更したいシーンを入れてね)
	sceneChanger->ChangeScene(SceneTitle);
	switch (selectCount)
	{
	case 0:
		sceneChanger->ChangeScene(SceneTitle);
		break;
	case 1:
		sceneChanger->ChangeScene(SceneGame);
		break;
	}
}
