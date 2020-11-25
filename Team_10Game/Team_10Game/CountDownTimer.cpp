#include "CountDownTimer.h"

CountDownTimer::CountDownTimer()
{
	Initialize();
}

CountDownTimer::CountDownTimer(float time)
	:Timer(time)
{
	//生成時に値を入れちゃう
	SetTime(time);
}

CountDownTimer::~CountDownTimer()
{
}

void CountDownTimer::Initialize()
{
	//現在時間に、制限時間を入れて初期化
	currentTime = limitTime;
}

void CountDownTimer::Update()
{
	//時間を減らす。
	currentTime -= 1.0f;
}

void CountDownTimer::SetTime(const float second)
{
	//時間をセット!
	limitTime = 60.0f * second;

	Initialize();
}

bool CountDownTimer::IsTime()
{
	return currentTime <= 0.0f;
}

float CountDownTimer::CurrentTime()
{
	return currentTime / 60.0f;
}

float CountDownTimer::Rate()
{
	return 1.0f - currentTime / limitTime;
}
