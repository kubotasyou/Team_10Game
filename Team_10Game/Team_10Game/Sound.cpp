#include "Sound.h"

#include <fstream>
#include <cassert>

#pragma comment(lib, "xaudio2.lib")//XAudio2�̃��C�u����
#pragma comment(lib, "strmiids.lib")//DirectShow�̃��C�u����

using namespace std;

Sound::Sound()
{
	Initialize();
	DirectShowInit();
}

Sound::~Sound()
{
	//�}�b�v���̂��ׂ�(for�Ƃ�)�̉����f�[�^�̃o�b�t�@�ЂƂÂ�delete����
	delete soundList[0].pBuffer;


	//XAudio2�̉��
	audio->Release();
	masterVoice->DestroyVoice();
}

void Sound::LoadSE(const std::string& filename)
{
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	const std::string fileN = filename;
	const std::string soundName = fileN + ".wav";
	const std::string directoryName = "Resources/Sound/";

	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(directoryName + soundName, std::ios_base::binary);
	//file.open(filename, std::ios_base::binary);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) { assert(0 && "�t�@�C���I�[�v���Ɏ��s"); }

	//�f�[�^�i�[�p�\����
	WaveData wavData;

	//RIFF�w�b�_�[�̓ǂݍ���
	/*RIFFHeader riffHeader;*/
	file.read((char*)&wavData.riffHeader, sizeof(wavData.riffHeader));
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(wavData.riffHeader.chunk.ID, "RIFF", 4) != 0) { assert(0 && "�t�@�C����RIFF�ł͂Ȃ�"); }

	//Format�`�����N�̓ǂݍ���
	/*FormatChunk format;*/
	file.read((char*)&wavData.format, sizeof(wavData.format));

	//Data�`�����N�̓ǂݍ���
	/*Chunk data;*/
	file.read((char*)&wavData.data, sizeof(wavData.data));


	//Data�`�����N�̔g�`�f�[�^�ǂݍ���
	/*char* */wavData.pBuffer = new char[wavData.data.size];
	file.read(wavData.pBuffer, wavData.data.size);

	//���X�g�ɖ��O�ƁA�f�[�^��ۑ�
	soundList.emplace(filename, wavData);

	//Wave�t�@�C�������
	file.close();
}

void Sound::Initialize()
{
	HRESULT result;

	//XAudio2������
	result = XAudio2Create(&audio);
	if FAILED(result) assert(0 && "XAudio2�����܂���ł����B");

	//�}�X�^�[�{�C�X
	result = audio->CreateMasteringVoice(&masterVoice);
	if FAILED(result) assert(0 && "MasteringVoice������܂���ł���");
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

void Sound::PlaySE(const std::string& filename, float volume)
{
	HRESULT result;

	//���O�ɑΉ������f�[�^�\���̂������Ă���
	WaveData& waveData = soundList[filename];

	WAVEFORMATEX waveFormatex{};

	//�g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&waveFormatex, &waveData.format.format, sizeof(waveData.format.format));
	waveFormatex.wBitsPerSample = waveData.format.format.nBlockAlign * 8 / waveData.format.format.nChannels;

	IXAudio2SourceVoice* source;

	//�g�`�t�H�[�}�b�g������ SourceVoice�̐���
	result = audio->CreateSourceVoice(
		&source,
		&waveFormatex
	);

	//�T�E���h�̐ݒ�\����
	XAUDIO2_BUFFER audioBuff{};

	//�Đ�����g�`�f�[�^��ݒ�
	audioBuff.pAudioData = (BYTE*)waveData.pBuffer;
	//pContext�Ƀo�b�t�@�̃A�h���X��n���B
	audioBuff.pContext = waveData.pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = waveData.data.size;

	//�n�܂�ꏊ���w��ł���B
	audioBuff.PlayBegin = 128 * 5;

	//�\�[�X�{�C�X�Ƀf�[�^�𑗐M
	result = source->SubmitSourceBuffer(&audioBuff);

	//���ʒ���(�ʏ��1.0)
	result = source->SetVolume(volume);

	//XAUDIO2_VOICE_STATE state;
	//source->GetState(&state);
	////�Đ����Ȃ�1������B
	//UINT tester = state.BuffersQueued;
	////�Đ����I�������1or0���Ԃ�݂�����
	//if (state.BuffersQueued <= 0)
	//{
	//	//�Đ����I����Ă���
	//	isPlay = true;
	//	UINT tester = state.BuffersQueued;
	//}
	//else
	//{
	//	//�Đ����Ă���
	//	isPlay = false;
	//}

	//�g�`�f�[�^�̍Đ�
	result = source->Start();

	//memo : �����Ŕz��ɒǉ�����̂��ĈӖ�����́H

	sourceData.emplace_back(source);
}

void Sound::Stop()
{
	for (auto a : sourceData)
	{
		a->Stop();
	}
}

void Sound::PlayLoop(const std::string & filename, float volume)
{
	HRESULT result;

	//���O�ɑΉ������f�[�^�\���̂������Ă���
	WaveData& test = soundList[filename];

	WAVEFORMATEX waveFormatex{};

	//�g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&waveFormatex, &test.format.format, sizeof(test.format.format));
	waveFormatex.wBitsPerSample = test.format.format.nBlockAlign * 8 / test.format.format.nChannels;

	IXAudio2SourceVoice* source;

	//�g�`�t�H�[�}�b�g������ SourceVoice�̐���
	result = audio->CreateSourceVoice(
		&source,
		&waveFormatex
	);

	//�T�E���h�̐ݒ�\����
	XAUDIO2_BUFFER audioBuff{};

	//�Đ�����g�`�f�[�^��ݒ�
	audioBuff.pAudioData = (BYTE*)test.pBuffer;
	//pContext�Ƀo�b�t�@�̃A�h���X��n���B
	audioBuff.pContext = test.pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = test.data.size;
	audioBuff.LoopCount = XAUDIO2_LOOP_INFINITE;

	//�\�[�X�{�C�X�Ƀf�[�^�𑗐M
	result = source->SubmitSourceBuffer(&audioBuff);

	//���ʒ���(�ʏ��1.0)
	result = source->SetVolume(volume);

	//�g�`�f�[�^�̍Đ�
	result = source->Start();

	sourceData.emplace_back(source);
}

//�t�@�C���̓ǂݍ��݁F�g���q�K�{
void Sound::DirectShowInit()
{
	HRESULT result;

	//COM�̏�����
	CoInitialize(NULL);

	//FillterGraph���쐬
	result = CoCreateInstance(
		CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&graphBuffer
	);
}

void Sound::LoadBGM(const std::string & filename)
{
	HRESULT result;

	//memo: �����Ń��[�J����MediaControl�����΂�������
	IMediaControl* mediaControl;//����Ă݂�
	//IMediaPosition* mediaPosition;//�Đ��ʒu�̎w��

	//MediaControl�̃C���^�[�t�F�[�X�擾
	result = graphBuffer->QueryInterface(
		IID_IMediaControl,
		(LPVOID*)&mediaControl
	);

	result = graphBuffer->QueryInterface(
		IID_IMediaPosition,
		(LPVOID*)&mediaPosition
	);


	const std::string directoryName = "Resources/Sound/";
	const std::string str = directoryName + filename;

	//�t�@�C������BSTR�ɕϊ�
	BSTR bstr = SysAllocString(StringToBSTR(str));

	//Graph���쐬
	result = mediaControl->RenderFile(bstr);
	if FAILED(result)
	{
		assert(0 && "�t�@�C�����ǂݍ��߂܂���ł����B");
	}

	//�ǂݍ��񂾂烊�X�g�Ɋi�[
	bgmList.emplace(filename, mediaControl);

	//�������
	SysFreeString(bstr);
	mediaControl->Release();
	mediaPosition->Release();
}

void Sound::PlayBGM(const std::string& filename)
{
	auto list = bgmList[filename];
	list->Run();
	isPlay = true;
	isLoop = false;
}

void Sound::PlayLoopBGM(const std::string& filename)
{
	auto list = bgmList[filename];
	list->Run();
	isPlay = true;
	isLoop = true;
}

void Sound::CheckLoop(const std::string& filename)
{
	//�Đ����ĂȂ����A���[�v���[�h����Ȃ���Ώ������Ȃ�
	if (!isPlay || !isLoop) return;

	REFTIME pos, end;
	mediaPosition->get_CurrentPosition(&pos);
	mediaPosition->get_StopTime(&end);
	if (pos >= end)
	{
		StopBGM();
		PlayLoopBGM(filename);
	}
}

void Sound::StopBGM()
{
	//�S�Ă�BGM�̒�~
	for (auto list : bgmList)
	{
		list.second->Stop();
	}
	mediaPosition->put_CurrentPosition(0);
	isPlay = false;
}

void Sound::Release()
{
	//�������
	graphBuffer->Release();
	CoUninitialize();
}
