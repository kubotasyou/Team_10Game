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


	//音声の再生
	//1回だけ流す場合はPlayBGMを初期化に書くだけでよい
	//sound->PlayMP3("BGM01.mp3");

	//ループ再生の場合、PlayLoopMP3を初期化に書き、
	//流したい音声ファイルを引数に入れる。
	//その後Updateへ...
	//sound->PlayLoopMP3("BGM01.mp3");

	//memo : 今使っているサウンドはこれ
	sound->PlayLoopWav("EndingDark");
}

void Ending::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE)||input->GetJoyPadTrigger(JoyPad::A))
	{
		NextScene();
	}

	//ループで流す場合、再生が終わったかを検知しないといけないため、
	//UpdeteにCheckLoop関数と、初期化に書いた音声ファイルを引数に入れる
	//これが無いとループしてくれないので注意。
	//sound->CheckLoop("BGM01.mp3");
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
		if (input->GetKeyTrigger(KeyCode::RIGHT)||input->GetStick("Vertices")>0)
		{
			selectCount = 1;
		}
	/*	if (input->GetKeyTrigger(KeyCode::LEFT)||input->GetStick("Vertices")<0)
		{
			selectCount = 1;
		}*/
		break;
	case 1:
		curssor->Draw();
		curssor->SetPosition({ 800.0f,490.0f });
		//if (input->GetKeyTrigger(KeyCode::RIGHT) || input->GetStick("Vertices") > 0)
		//{
		//	selectCount = 0;
		//}
		if (input->GetKeyTrigger(KeyCode::LEFT)||input->GetStick("Vertices")<0)
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
	sound->StopWav();

	//最後にシーンが終了するときに止めたい音声ファイルを選択して終わり。
	//sound->StopMP3("BGM01.mp3");

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
