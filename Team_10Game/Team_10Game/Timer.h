#pragma once

//タイマー基底クラス

class Timer
{
public:
	//コンストラクタ
	Timer();
	Timer(float time);//これ使うかな?

public:

	virtual ~Timer() {};//これなんで必要?
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void SetTime(const float second) = 0;
	virtual bool IsTime() = 0;
	virtual float CurrentTime() = 0;
	virtual float Rate() = 0;

protected:
	float limitTime;  //制限時間
	float currentTime;//現在時間
};

