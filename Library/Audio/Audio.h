#pragma once

#include <xaudio2.h>
#include "AudioSource.h"

// �I�[�f�B�I
class Audio
{
public:
	// �R���X�g���N�^
	Audio();

	// �f�X�g���N�^
	~Audio();

public:
	// �C���X�^���X�擾
	static Audio& Instance() { return *instance; }

	// �I�[�f�B�I�\�[�X�ǂݍ���
	std::unique_ptr<AudioSource> LoadAudioSource(const char* filename);

private:
	inline static Audio*	instance		= nullptr;

	IXAudio2*				xaudio			= nullptr;
	IXAudio2MasteringVoice* masteringVoice	= nullptr;
};
