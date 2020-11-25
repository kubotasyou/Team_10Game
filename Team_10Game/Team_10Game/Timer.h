#pragma once

//�^�C�}�[���N���X

class Timer
{
public:
	//�R���X�g���N�^
	Timer();
	Timer(float time);//����g������?

public:

	virtual ~Timer() {};//����Ȃ�ŕK�v?
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void SetTime(const float second) = 0;
	virtual bool IsTime() = 0;
	virtual float CurrentTime() = 0;
	virtual float Rate() = 0;

protected:
	float limitTime;  //��������
	float currentTime;//���ݎ���
};

