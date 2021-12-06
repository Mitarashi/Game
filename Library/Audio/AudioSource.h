#pragma once

#include <memory>
#include <xaudio2.h>

#include "AudioResource.h"

// �I�[�f�B�I�\�[�X
class AudioSource
{
public:
	// �R���X�g���N�^
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);

	// �f�X�g���N�^
	~AudioSource();

	// �Đ�
	void Play(bool loop);

	// ��~
	void Stop();

private:
	IXAudio2SourceVoice*			sourceVoice = nullptr;
	std::shared_ptr<AudioResource>	resource	= nullptr;
};
