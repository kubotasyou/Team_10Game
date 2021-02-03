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
	currentScene = (BaseScene*) new Title(this, dxManager, input, sound);

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
#pragma region テクスチャ読み込み

	// 0デバッグテキスト
	Sprite::LoadTexture(0, L"Resources/Texture/debugfont.png");
	// 1フェード用黒画像
	Sprite::LoadTexture(1, L"Resources/Texture/black.png");
	// 2タイトルの画像
	Sprite::LoadTexture(2, L"Resources/Texture/Title.png");
	// 3エンディングの画像
	Sprite::LoadTexture(3, L"Resources/Texture/Ending.png");
	// 4カーソルの画像
	Sprite::LoadTexture(4, L"Resources/Texture/curssor.png");
	// 5ゲームシーン背景
	Sprite::LoadTexture(5, L"Resources/Texture/Space.jpg");
	// 6ポインターの画像
	Sprite::LoadTexture(6, L"Resources/Texture/marker.png");
#pragma endregion

#pragma region サウンド読み込み

	sound->LoadWav("3MinutesCooking");
	sound->LoadWav("select");
	sound->LoadWav("clear");
	sound->LoadWav("Title");
	sound->LoadWav("Gameplay");
	sound->LoadWav("Ending");
	sound->LoadWav("EndingDark");

	//MP3ファイルの読み込み
	//識別子(.mp3)が必要
	//この時にファイル名をコピーしておくと良い
	sound->LoadMP3("BGM01.mp3");

#pragma endregion

	//シーンの初期化(読み込みはこれより↑に書く)
	currentScene->Initialize();

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
			currentScene = (BaseScene*) new GamePlay(this, dxManager, input,score,sound);
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
	fader->SetFadeIn(0.5f);  //フェードの時間指定
	fader->SwitchFade(false);//フェードイン
	this->nextScene = nextScene;
}
