#pragma once

#include <vector>
#include <Windows.h>

// �I�[�f�B�I���\�[�X
class AudioResource
{
public:
	// �R���X�g���N�^
	AudioResource(const char* filename);

	// �f�X�g���N�^
	~AudioResource() = default;

	// �f�[�^�擾
	UINT8* GetAudioData() { return data.data(); }

	// �f�[�^�T�C�Y�擾
	UINT32 GetAudioBytes() const { return static_cast<UINT32>(data.size()); }

	// WAVE�t�H�[�}�b�g�擾
	const WAVEFORMATEX& GetWaveFormat() const { return wfx; }

private:
	// RIFF�w�b�_
	struct Riff
	{
		UINT32				tag		= 0; // RIFF�`���̎��ʎq 'RIFF'
		UINT32				size	= 0; // ����ȍ~�̃t�@�C���T�C�Y(�t�@�C���T�C�Y - 8)
		UINT32				type	= 0; // RIFF�̎�ނ�\�����ʎq 'WAVE'
	};

	// �`�����N
	struct Chunk
	{
		UINT32				tag		= 0; // �`�����N�`���̎��ʎq 'fmt ' 'data'
		UINT32				size	= 0; // �f�[�^�T�C�Y('fmt '���j�APCM�Ȃ��16 'data'�g�`�f�[�^�T�C�Y)
	};

	// fmt �`�����N
	struct Fmt
	{
		UINT16				fmtId		= 0; // �t�H�[�}�b�gID(���j�APCM�Ȃ��1)
		UINT16				channel		= 0; // �`�����l����(���m����:1 �X�e���I:2)
		UINT32				sampleRate	= 0; // �T���v�����O���[�g(44.1kHz�Ȃ�44100)
		UINT32				transRate	= 0; // �f�[�^���x(Byte/sec) [ 44.1kHz 16bit �X�e���I 44100�~2�~2 ]
		UINT16				blockSize	= 0; // �u���b�N�T�C�Y(Byte/sample�~�`�����l����)
		UINT16				quantumBits = 0; // �T���v��������̃r�b�g��(bit/sample)
	};

	Riff					riff	= {};
	Fmt						fmt		= {};
	std::vector<UINT8>		data	= {};
	WAVEFORMATEX			wfx		= {};
};
