#include "Score.h"

#include <sstream>
#include <iomanip>

Score::Score(DirectXManager * dxManager)
	:dxManager(dxManager)
{
	//デバッグテキスト初期化
	scoreText.Initialize(0);
}

Score::~Score()
{
}

void Score::Initialize()
{
	//スコアを初期化
	currentScore = 0;
	scoreValue = 0;
}

void Score::Update()
{
	//受け取った値が0より大きいとき
	if (scoreValue > 0)
	{
		scoreValue -= 1;
		currentScore += 1;
	}
}

void Score::Draw(float x, float y, float scale, DirectX::XMFLOAT4 color)
{
	std::ostringstream scorestr;
	scorestr.clear();
	scorestr << "Score:" << std::fixed << currentScore;
	scoreText.Print(scorestr.str(), x, y, scale, color);

	scoreText.DrawAll(dxManager->GetcmdList());
}

void Score::AddScore(int score)
{
	int value = score;
	scoreValue += value;
}
