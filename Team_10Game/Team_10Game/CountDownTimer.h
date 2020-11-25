#pragma once
#include "Timer.h"

//時間計測クラス(制限時間制)
class CountDownTimer : public Timer
{
public:
	//コンストラクタ
	CountDownTimer();
	CountDownTimer(float time);

	~CountDownTimer();

	void Initialize() override;
	void Update() override;
	void SetTime(const float second) override;
	bool IsTime() override;

public:
	float CurrentTime();
	float Rate();
};

