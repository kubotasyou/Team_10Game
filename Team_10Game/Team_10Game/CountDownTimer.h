#pragma once
#include "Timer.h"

//���Ԍv���N���X(�������Ԑ�)
class CountDownTimer : public Timer
{
public:
	//�R���X�g���N�^
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

