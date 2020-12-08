#pragma once
#include <Windows.h>
#include <wrl.h>

//xaudio�̃C���N���[�h
#include <xaudio2.h>

#include <unordered_map>

using namespace Microsoft::WRL;

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

	//1�����ɂ�1�K�v�ȃf�[�^
	struct WaveData
	{
		RIFFHeader riffHeader;
		FormatChunk format;
		Chunk data;
		char* pBuffer = nullptr;
	};

public:
	Sound();
	~Sound();

	void LoadSound(const std::string& filename);

	//�Đ�(�t�@�C�����E����(�ʏ��1.0))
	void Play(const std::string& filename, float volume = 1.0f);

	//���[�v�Đ�(�t�@�C����)
	void PlayLoop(const std::string& filename, float volume = 1.0f);

private:
	void Initialize();

private:
	//Xaudio2����
	IXAudio2 *audio;                    
	//MasteringVoice����
	IXAudio2MasteringVoice* masterVoice;

	//�ǂݍ��񂾉��f�[�^
	std::unordered_map<std::string, WaveData> soundList;

	//�Đ����鉹��
	std::vector<IXAudio2SourceVoice*> sourceData;

	bool isPlay = false;
};

