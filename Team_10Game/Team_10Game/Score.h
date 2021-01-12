#pragma once

#include "DirectXManager.h"
#include "DebugText.h"

//スコア表示クラス(2Dテクスチャ)
class Score
{
public:
	Score(DirectXManager * dxManager);
	~Score();

	void Initialize();

	void Update();

	void Draw(float x, float y, float scale, DirectX::XMFLOAT4 color = { 1,1,1,1 });

	//現在のスコアを取得(これいらないかも)
	int GetScore() { return currentScore; }
	//現在のスコアを増やす。
	void AddScore(int score);

private:
	DirectXManager * dxManager;
	DebugText scoreText;

	int currentScore;//現在のスコア
	int scoreValue;  //加算されるスコア
};

