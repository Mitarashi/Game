#pragma once

#include <xaudio2.h>
#include "AudioSource.h"

// オーディオ
class Audio
{
public:
	// コンストラクタ
	Audio();

	// デストラクタ
	~Audio();

public:
	// インスタンス取得
	static Audio& Instance() { return *instance; }

	// オーディオソース読み込み
	std::unique_ptr<AudioSource> LoadAudioSource(const char* filename);

private:
	inline static Audio*	instance		= nullptr;

	IXAudio2*				xaudio			= nullptr;
	IXAudio2MasteringVoice* masteringVoice	= nullptr;
};
