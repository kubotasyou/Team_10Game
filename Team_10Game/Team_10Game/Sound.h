#pragma once
#include <Windows.h>
#include <wrl.h>

//xaudio�̃C���N���[�h
#include <xaudio2.h>

using namespace Microsoft::WRL;

class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	////�{�C�X�����p�X�̊J�n
	//void OnVoiceProcessingPassStart(UINT32 byteRequired) {};
	////�{�C�X�����p�X�̏I����
	//void OnVoiceProcessingPassEnd() {};
	////�o�b�t�@�X�g���[���̍Đ����I�������Ƃ�
	//void OnStreamEnd() {};
	////�o�b�t�@�̎g�p�J�n��
	//void OnBufferStart(void* pBufferContext) {};
	////�o�b�t�@�̖����ɓ��B�����Ƃ�
	//void OnBufferEnd(void* pBufferContext)
	//{
	//	//�o�b�t�@���������
	//	delete[] pBufferContext;
	//}
	////�Đ������[�v�ʒu�ɒB�����Ƃ�
	//void OnLoopEnd(void* pBufferContext) {};
	////�{�C�Xr�̎��s�G���[��
	//void OnvoiceError(void* pBufferContext, HRESULT error) {};

	//�{�C�X�����p�X�̊J�n
	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
	//�{�C�X�����p�X�̏I����
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};

	//�o�b�t�@�X�g���[���̍Đ����I�������Ƃ�
	STDMETHOD_(void, OnStreamEnd)(THIS) {};
	//�o�b�t�@�̎g�p�J�n��
	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
	//�o�b�t�@�̖����ɓ��B�����Ƃ�
	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext)
	{
		//�o�b�t�@���������
		delete[] pBufferContext;
	}

	//�Đ������[�v�ʒu�ɒB�����Ƃ�
	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContext) {};

	//�{�C�Xr�̎��s�G���[��
	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContext, HRESULT error) {};
};

class Sound
{
public:
	//�e�`�����NID��ǂݎ�邽�߂̍\����

	//�`�����N�w�b�_
	struct Chunk
	{
		char ID[4];  //�e�`�����N��ID
		int size;//�`�����N�T�C�Y
	};

	//RIFF�`�����N
	struct RIFFHeader
	{
		Chunk chunk;//RIFF
		char type[4];//Wave
	};

	//FMT�`�����N
	struct FormatChunk
	{
		Chunk chunk;      //fmt
		WAVEFORMAT format;//�g�`�t�H�[�}�b�g
	};

public:
	Sound();
	~Sound();

	void Play(const char* filename);

private:
	void Initialize();

private:
	IXAudio2 *audio;
	IXAudio2MasteringVoice* masterVoice;
	//�C���X�^���X
	XAudio2VoiceCallback voiceCallback;
};

