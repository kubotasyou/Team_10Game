#pragma once
#include <Windows.h>
#include <wrl.h>

//xaudioのインクルード
#include <xaudio2.h>

#include <unordered_map>

using namespace Microsoft::WRL;

class Sound
{
public:
	//各チャンクIDを読み取るための構造体

	//チャンクヘッダ
	struct Chunk
	{
		char ID[4];  //各チャンクのID
		int size;//チャンクサイズ
	};

	//RIFFチャンク
	struct RIFFHeader
	{
		Chunk chunk;//RIFF
		char type[4];//Wave
	};

	//FMTチャンク
	struct FormatChunk
	{
		Chunk chunk;      //fmt
		WAVEFORMAT format;//波形フォーマット
	};

	//1音声につき1つ必要なデータ
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

	//再生(ファイル名・音量(通常は1.0))
	void Play(const std::string& filename, float volume = 1.0f);

	//ループ再生(ファイル名)
	void PlayLoop(const std::string& filename, float volume = 1.0f);

private:
	void Initialize();

private:
	//Xaudio2生成
	IXAudio2 *audio;                    
	//MasteringVoice生成
	IXAudio2MasteringVoice* masterVoice;

	//読み込んだ音データ
	std::unordered_map<std::string, WaveData> soundList;

	//再生する音声
	std::vector<IXAudio2SourceVoice*> sourceData;

	bool isPlay = false;
};

