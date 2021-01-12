#include "Score.h"

#include <sstream>
#include <iomanip>

Score::Score(DirectXManager * dxManager)
	:dxManager(dxManager)
{
	//�f�o�b�O�e�L�X�g������
	scoreText.Initialize(0);
}

Score::~Score()
{
}

void Score::Initialize()
{
	//�X�R�A��������
	currentScore = 0;
	scoreValue = 0;
}

void Score::Update()
{
	//�󂯎�����l��0���傫���Ƃ�
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
