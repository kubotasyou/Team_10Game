#pragma once
#include <Windows.h>
#include <wrl.h>

//xaudioのインクルード
#include <xaudio2.h>
//DirectShowのインクルード
#include <dshow.h>

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

#pragma region XAudio2

	//wavファイルの読み込み(ファイル名(識別子無し))
	void LoadWav(const std::string& filename);

	//wav再生(ファイル名(識別子無し)・音量(通常は1.0))
	void PlayWav(const std::string& filename, float volume = 1.0f);

	//wav音声終了
	void StopWav();

	//wavループ再生(ファイル名(識別子無し))
	void PlayLoopWav(const std::string& filename, float volume = 1.0f);

#pragma endregion

#pragma region DirectShow

	//DirectShow初期化
	void DirectShowInit();

	//MP3ファイルの読み込み(ファイル名(識別子必要))
	void LoadMP3(const std::string& filename);

	//MP3再生
	void PlayMP3(const std::string& filename);

	//MP3ループ再生
	void PlayLoopMP3(const std::string& filename);
	void CheckLoop(const std::string& filename);

	//MP3停止
	void StopMP3(const std::string& filename);

	void Release();

#pragma endregion

private:

	//XAudio2初期化
	void Initialize();

	//stringからBSTRに変換
	BSTR StringToBSTR(const std::string& str);

private:
	//Xaudio2生成
	IXAudio2 *audio;                    
	//MasteringVoice生成
	IXAudio2MasteringVoice* masterVoice;
	//読み込んだ音データ
	std::unordered_map<std::string, WaveData> soundList;
	//再生する音声
	std::vector<IXAudio2SourceVoice*> sourceData;

	//↓DirectShow
	//GraphBuilderリスト(ファイル名・GraphBuilder)
	std::unordered_map<std::string, IGraphBuilder*> builderList;

	//MediaControlリスト(ファイル名・MediaControl)
	std::unordered_map<std::string, IMediaControl*> controlList;

	//MediaPositionリスト(ファイル名・MediaPosition)
	std::unordered_map<std::string, IMediaPosition*> positionList;

	bool isPlay = false;//再生しているか
	bool isLoop = false;//ループ再生するか
};

