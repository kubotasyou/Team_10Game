#include "CountDownTimer.h"

CountDownTimer::CountDownTimer()
{
	Initialize();
}

CountDownTimer::CountDownTimer(float time)
	:Timer(time)
{
	//�������ɒl����ꂿ�Ⴄ
	SetTime(time);
}

CountDownTimer::~CountDownTimer()
{
}

void CountDownTimer::Initialize()
{
	//���ݎ��ԂɁA�������Ԃ����ď�����
	currentTime = limitTime;
}

void CountDownTimer::Update()
{
	//���Ԃ����炷�B
	currentTime -= 1.0f;
}

void CountDownTimer::SetTime(const float second)
{
	//���Ԃ��Z�b�g!
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
