#include "SceneManager.h"
#include "SafeDelete.h"
#include "Title.h"
#include "Ending.h"
#include "GamePlay.h"

#include "Sprite.h"

SceneManager::SceneManager(DirectXManager * manager, Input * input, Sound* sound)
	:nextScene(SceneNone),    //最初は何も入れないでおく
	dxManager(manager),
	input(input),
	sound(sound),
	fader(new Fader(manager)),//フェーダー生成
	score(new Score(dxManager))
{
	//最初のシーンを設定
	currentScene = (BaseScene*) new GamePlay(this, dxManager, input,score);

	//フェードのタイプを設定
	fader->SetFaderType(FadeType::Normal);

	//スコアを初期化
	score->Initialize();
}

SceneManager::~SceneManager()
{
	delete(currentScene);
	delete(fader);
	delete(score);
}

void SceneManager::Initialize()
{
	currentScene->Initialize();

	// 0デバッグテキスト
	Sprite::LoadTexture(0, L"Resources/Texture/debugfont.png");
	// 1フェード用黒画像
	Sprite::LoadTexture(1, L"Resources/Texture/black.png");
	// 5タイトルの画像
	Sprite::LoadTexture(2, L"Resources/Texture/Title.png");
	// 6エンディングの画像
    Sprite::LoadTexture(3, L"Resources/Texture/Ending.png");
}

void SceneManager::Update()
{
	fader->Update();

	//フェード中だったら処理をしない
	if (fader->GetisFade()) return;

	//次のシーンがNoneじゃなかったら処理
	if (nextScene != SceneNone)
	{
		//現在のシーン終了
		delete currentScene;

		switch (nextScene)
		{
		case SceneTitle:
			//Titleの第一引数はISceneChanger;
			//SceneManagerもISceneChangerを継承するものなので
			//thisを入れることで、TitleシーンでSceneManagerを使える!
			currentScene = (BaseScene*) new Title(this, dxManager, input,sound);
			break;
		case SceneGame:
			currentScene = (BaseScene*) new GamePlay(this, dxManager, input,score);
			break;
		case SceneEnding:
			currentScene = (BaseScene*) new Ending(this, dxManager, input,sound,score);
			break;

		default:
			break;
		}

		//別のシーンを呼んだらNoneシーンで初期化
		nextScene = SceneNone;
		currentScene->Initialize();
		//一秒かけてフェードアウト
		fader->SetFadeOut(0.5f);
		fader->SwitchFade(true);
	}

	//フェード中だったら処理をしない
	if (fader->GetisFade()) return;

	currentScene->Update();
}

void SceneManager::Draw()
{
	currentScene->Draw();
	fader->Draw();
}

void SceneManager::ChangeScene(SceneType nextScene)
{
	fader->SetFadeIn(0.5f);
	fader->SwitchFade(false);
	this->nextScene = nextScene;
}
