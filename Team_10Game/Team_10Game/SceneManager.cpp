#include "SceneManager.h"
#include "SafeDelete.h"
#include "Title.h"
#include "Ending.h"
#include "GamePlay.h"

SceneManager::SceneManager(DirectXManager * manager, Input * input, Sound* sound)
	:nextScene(SceneNone),    //最初は何も入れないでおく
	dxManager(manager),
	input(input),
	sound(sound),
	fader(new Fader(manager))//フェーダー生成
{
	//最初のシーンを設定
	currentScene = (BaseScene*) new GamePlay(this, dxManager, input);
	fader->SetFaderType(FadeType::Normal);
}

SceneManager::~SceneManager()
{
	delete(currentScene);
	delete(fader);
}

void SceneManager::Initialize()
{
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
			currentScene = (BaseScene*) new GamePlay(this, dxManager, input);
			break;
		case SceneEnding:
			currentScene = (BaseScene*) new Ending(this, dxManager, input,sound);
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
	//ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();
	//Sprite::BeginDraw(cmdList);
	//GameObject::BeginDraw(cmdList);
	currentScene->Draw();
	fader->Draw();
	//GameObject::EndDraw();
	// スプライト描画後処理
	//Sprite::EndDraw();
}

void SceneManager::ChangeScene(SceneType nextScene)
{
	fader->SetFadeIn(0.5f);
	fader->SwitchFade(false);
	this->nextScene = nextScene;
}
