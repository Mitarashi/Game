#pragma once

#include <windows.h>
#include <crtdbg.h>

// �x���`�}�[�N�N���X
class Benchmark
{
public:
	// �R���X�g���N�^
	Benchmark();

	// �f�X�g���N�^
	~Benchmark() = default;

	// �J�n�ʒu
	void Begin();

	// �I���ʒu
	float End();

private:
	LARGE_INTEGER ticksPerSecond	= {};
	LARGE_INTEGER startTicks		= {};
	LARGE_INTEGER currentTicks		= {};
};
