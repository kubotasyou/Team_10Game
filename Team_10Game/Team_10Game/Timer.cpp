#include "Timer.h"

Timer::Timer()
{
}

Timer::Timer(float time)
{
	//制限時間設定
	limitTime = 60.0f * time;
}
