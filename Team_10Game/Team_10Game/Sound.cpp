#include "Sound.h"

#include <fstream>
#include <cassert>

#pragma comment(lib, "xaudio2.lib")//XAudio2のライブラリ
#pragma comment(lib, "strmiids.lib")//DirectShowのライブラリ

using namespace std;

Sound::Sound()
{
	Initialize();    //XAudio2初期化
	DirectShowInit();//DirectShow初期化
}

Sound::~Sound()
{
	//マップ内のすべて(forとか)の音声データのバッファひとつづつをdeleteする
	delete soundList[0].pBuffer;

	//XAudio2の解放
	audio->Release();
	masterVoice->DestroyVoice();
}

void Sound::LoadWav(const std::string& filename)
{
	//ファイル入力ストリームのインスタンス
	std::ifstream file;

	const std::string fileN = filename;
	const std::string soundName = fileN + ".wav";
	const std::string directoryName = "Resources/Sound/";

	// .wavファイルをバイナリモードで開く
	file.open(directoryName + soundName, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	if (file.fail()) { assert(0 && "ファイルオープンに失敗"); }

	//データ格納用構造体
	WaveData wavData;

	//RIFFヘッダーの読み込み
	file.read((char*)&wavData.riffHeader, sizeof(wavData.riffHeader));
	// ファイルがRIFFかチェック
	if (strncmp(wavData.riffHeader.chunk.ID, "RIFF", 4) != 0) { assert(0 && "ファイルがRIFFではない"); }

	//Formatチャンクの読み込み
	file.read((char*)&wavData.format, sizeof(wavData.format));

	//Dataチャンクの読み込み
	file.read((char*)&wavData.data, sizeof(wavData.data));

	//Dataチャンクの波形データ読み込み
    wavData.pBuffer = new char[wavData.data.size];
	file.read(wavData.pBuffer, wavData.data.size);

	//リストに名前と、データを保存
	soundList.emplace(filename, wavData);

	//Waveファイルを閉じる
	file.close();
}

void Sound::Initialize()
{
	HRESULT result;

	//XAudio2初期化
	result = XAudio2Create(&audio);
	if FAILED(result) assert(0 && "XAudio2が作れませんでした。");

	//マスターボイス
	result = audio->CreateMasteringVoice(&masterVoice);
	if FAILED(result) assert(0 && "MasteringVoiceがつくれませんでした");
}

BSTR Sound::StringToBSTR(const std::string & str)
{
	int wslen = ::MultiByteToWideChar(
		CP_ACP,
		0,
		str.data(),
		static_cast<int>(str.length()),
		NULL,
		0
	);

	BSTR wsdata = SysAllocStringLen(
		NULL,
		wslen
	);

	MultiByteToWideChar(
		CP_ACP,
		0,
		str.data(),
		static_cast<int>(str.length()),
		wsdata,
		wslen
	);

	return wsdata;
}

void Sound::PlayWav(const std::string& filename, float volume)
{
	HRESULT result;

	//名前に対応したデータ構造体を持ってくる
	WaveData& waveData = soundList[filename];

	WAVEFORMATEX waveFormatex{};

	//波形フォーマットの設定
	memcpy(&waveFormatex, &waveData.format.format, sizeof(waveData.format.format));
	waveFormatex.wBitsPerSample = waveData.format.format.nBlockAlign * 8 / waveData.format.format.nChannels;

	IXAudio2SourceVoice* source;

	//波形フォーマットを元に SourceVoiceの生成
	result = audio->CreateSourceVoice(
		&source,
		&waveFormatex
	);

	//サウンドの設定構造体
	XAUDIO2_BUFFER audioBuff{};

	//再生する波形データを設定
	audioBuff.pAudioData = (BYTE*)waveData.pBuffer;
	//pContextにバッファのアドレスを渡す。
	audioBuff.pContext = waveData.pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = waveData.data.size;

	//始まる場所を指定できる。
	audioBuff.PlayBegin = 128 * 5;

	//ソースボイスにデータを送信
	result = source->SubmitSourceBuffer(&audioBuff);

	//音量調節(通常は1.0)
	result = source->SetVolume(volume);

	//XAUDIO2_VOICE_STATE state;
	//source->GetState(&state);
	////再生中なら1が入る。
	//UINT tester = state.BuffersQueued;
	////再生が終わったら1or0が返るみたいな
	//if (state.BuffersQueued <= 0)
	//{
	//	//再生が終わっている
	//	isPlay = true;
	//	UINT tester = state.BuffersQueued;
	//}
	//else
	//{
	//	//再生している
	//	isPlay = false;
	//}

	//波形データの再生
	result = source->Start();

	//memo : ここで配列に追加するのって意味あるの？

	sourceData.emplace_back(source);
}

void Sound::StopWav()
{
	for (auto a : sourceData)
	{
		a->Stop();
	}
}

void Sound::PlayLoopWav(const std::string & filename, float volume)
{
	HRESULT result;

	//名前に対応したデータ構造体を持ってくる
	WaveData& test = soundList[filename];

	WAVEFORMATEX waveFormatex{};

	//波形フォーマットの設定
	memcpy(&waveFormatex, &test.format.format, sizeof(test.format.format));
	waveFormatex.wBitsPerSample = test.format.format.nBlockAlign * 8 / test.format.format.nChannels;

	IXAudio2SourceVoice* source;

	//波形フォーマットを元に SourceVoiceの生成
	result = audio->CreateSourceVoice(
		&source,
		&waveFormatex
	);

	//サウンドの設定構造体
	XAUDIO2_BUFFER audioBuff{};

	//再生する波形データを設定
	audioBuff.pAudioData = (BYTE*)test.pBuffer;
	//pContextにバッファのアドレスを渡す。
	audioBuff.pContext = test.pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = test.data.size;
	audioBuff.LoopCount = XAUDIO2_LOOP_INFINITE;

	//ソースボイスにデータを送信
	result = source->SubmitSourceBuffer(&audioBuff);

	//音量調節(通常は1.0)
	result = source->SetVolume(volume);

	//波形データの再生
	result = source->Start();

	sourceData.emplace_back(source);
}

//ファイルの読み込み：拡張子必須
void Sound::DirectShowInit()
{
	HRESULT result;

	//COMの初期化
	CoInitialize(NULL);
}

void Sound::LoadMP3(const std::string & filename)
{
	HRESULT result;

	//GraphBuilderを作成
	IGraphBuilder* graphBuilder = nullptr;
	result = CoCreateInstance(
		CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&graphBuilder
	);

	//MediaControlを作成
	IMediaControl* mediaControl = nullptr;//一応nullで初期化
	result = graphBuilder->QueryInterface(
		IID_IMediaControl,
		(LPVOID*)&mediaControl
	);

	//MediaPositionを作成
	IMediaPosition* mediaPosition = nullptr;//nullで初期化
	result = graphBuilder->QueryInterface(
		IID_IMediaPosition,
		(LPVOID*)&mediaPosition
	);

	const std::string directoryName = "Resources/Sound/";
	const std::string str = directoryName + filename;

	//ファイル名をBSTRに変換
	BSTR bstr = SysAllocString(StringToBSTR(str));

	//ローカルなMediaControlで、ファイルを読み込む。
	result = mediaControl->RenderFile(bstr);
	if FAILED(result)
	{
		assert(0 && "ファイルが読み込めませんでした。");
	}

	//ファイルが読み込めたらそれぞれのリストに格納
	builderList.emplace(filename, graphBuilder);
	controlList.emplace(filename, mediaControl);
	positionList.emplace(filename, mediaPosition);

	//解放処理
	SysFreeString(bstr);

	//入れ終わったからもういらない
	mediaPosition->Release();
	mediaControl->Release();
	//testGraphBuilder->Release();
}

void Sound::PlayMP3(const std::string& filename)
{
	//名前と一緒に格納したcontrolを取り出す
	auto conList = controlList[filename];
	conList->Run();//再生

	isPlay = true;
	isLoop = false;
}

void Sound::PlayLoopMP3(const std::string& filename)
{
	//名前と一緒に格納したcontrolを取り出す
	auto conList = controlList[filename];
	conList->Run();//再生

	isPlay = true;
	isLoop = true;
}

void Sound::CheckLoop(const std::string& filename)
{
	//再生してないか、ループモードじゃなければ処理しない
	if (!isPlay || !isLoop) return;

	//名前と一緒に格納したpositionを取り出す
	auto posList = positionList[filename];


	REFTIME pos, end;
	posList->get_CurrentPosition(&pos);
	posList->get_StopTime(&end);
	if (pos >= end)
	{
		//この方法で戻すと開始が少し途切れてしまった
		//pMediaPosition->put_CurrentPosition(0);
		//pMediaControl->Run();

		//そこでいったん止めてから再生する
		StopMP3(filename);
		PlayLoopMP3(filename);
	}
}

void Sound::StopMP3(const std::string& filename)
{
	//名前と一緒に格納したcontrolを取り出す
	auto conList = controlList[filename];

	conList->Stop();//停止

	//名前と一緒に格納したpositionを取り出す
	auto posList = positionList[filename];

	//再生位置を最初に戻す
	posList->put_CurrentPosition(0);
	isPlay = false;
}

void Sound::Release()
{
	//解放処理
	CoUninitialize();
}
