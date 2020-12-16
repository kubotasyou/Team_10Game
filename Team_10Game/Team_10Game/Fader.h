#pragma once
#include "DirectXManager.h"
#include "Sprite.h"
#include "WinApp.h"

enum FadeType
{
	Normal,   //通常のフェード
	ScrollX,  //横方向にスクロール
	ScrollY,  //縦方向にスクロール
	Expansion,//拡大
};

//フェード処理
class Fader
{
public:
	//コンスト
	Fader(DirectXManager* manager);
	//デスト
	~Fader();

	void Initialize();

	void Update();

	void Draw();

	//フェードの種類を設定
	void SetFaderType(FadeType type);
	//フェードインの準備(準備時間)
	void SetFadeIn(float setTime);
	//フェードアウトの準備(準備時間)
	void SetFadeOut(float setTime);
	//状態の切り替え(false : fadeIn, true : fadeOut)
	bool SwitchFade(bool value);
	//フェードしているか?
	bool GetisFade() { return isFade; }

private:
	//フェードイン(setTime_in)
	void FadeIn(float speed);
	//フェードアウト(setTime_out)
	void FadeOut(float speed);

private:
	DirectXManager* dxManager = nullptr;
	Sprite* sprite = nullptr;

	float alpha;      //画像のα値
	float setTime_in; //フェードに入る時間
	float setTime_out;//フェードが終わる時間
	bool inOut;       //true : fadeOut, false : fadeIn
	bool isFade;      //フェードしているか？

	FadeType fadeType;
	float width, height, ex;

	const float window_width = WinApp::window_width;  //画面横
	const float winddow_height = WinApp::window_height;//画面縦
};

