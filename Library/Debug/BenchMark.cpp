#include "BenchMark.h"

// �R���X�g���N�^
Benchmark::Benchmark()
{
	QueryPerformanceFrequency(&ticksPerSecond);
	QueryPerformanceCounter(&startTicks);
	QueryPerformanceCounter(&currentTicks);
}

// �J�n�ʒu
void Benchmark::Begin()
{
	QueryPerformanceCounter(&startTicks);
}

// �I���ʒu
float Benchmark::End()
{
	QueryPerformanceCounter(&currentTicks);
	return static_cast<float>(currentTicks.QuadPart - startTicks.QuadPart) /
		static_cast<float>(ticksPerSecond.QuadPart);
}
