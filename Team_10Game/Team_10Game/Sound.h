#pragma once
#include <Windows.h>
#include <wrl.h>

//xaudio�̃C���N���[�h
#include <xaudio2.h>
//DirectShow�̃C���N���[�h
#include <dshow.h>

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

#pragma region XAudio2

	//wav�t�@�C���̓ǂݍ���(�t�@�C����(���ʎq����))
	void LoadWav(const std::string& filename);

	//wav�Đ�(�t�@�C����(���ʎq����)�E����(�ʏ��1.0))
	void PlayWav(const std::string& filename, float volume = 1.0f);

	//wav�����I��
	void StopWav();

	//wav���[�v�Đ�(�t�@�C����(���ʎq����))
	void PlayLoopWav(const std::string& filename, float volume = 1.0f);

#pragma endregion

#pragma region DirectShow

	//DirectShow������
	void DirectShowInit();

	//MP3�t�@�C���̓ǂݍ���(�t�@�C����(���ʎq�K�v))
	void LoadMP3(const std::string& filename);

	//MP3�Đ�
	void PlayMP3(const std::string& filename);

	//MP3���[�v�Đ�
	void PlayLoopMP3(const std::string& filename);
	void CheckLoop(const std::string& filename);

	//MP3��~
	void StopMP3(const std::string& filename);

	void Release();

#pragma endregion

private:

	//XAudio2������
	void Initialize();

	//string����BSTR�ɕϊ�
	BSTR StringToBSTR(const std::string& str);

private:
	//Xaudio2����
	IXAudio2 *audio;                    
	//MasteringVoice����
	IXAudio2MasteringVoice* masterVoice;
	//�ǂݍ��񂾉��f�[�^
	std::unordered_map<std::string, WaveData> soundList;
	//�Đ����鉹��
	std::vector<IXAudio2SourceVoice*> sourceData;

	//��DirectShow
	//GraphBuilder���X�g(�t�@�C�����EGraphBuilder)
	std::unordered_map<std::string, IGraphBuilder*> builderList;

	//MediaControl���X�g(�t�@�C�����EMediaControl)
	std::unordered_map<std::string, IMediaControl*> controlList;

	//MediaPosition���X�g(�t�@�C�����EMediaPosition)
	std::unordered_map<std::string, IMediaPosition*> positionList;

	bool isPlay = false;//�Đ����Ă��邩
	bool isLoop = false;//���[�v�Đ����邩
};

