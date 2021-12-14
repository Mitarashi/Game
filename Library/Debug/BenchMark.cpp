#include "BenchMark.h"

// コンストラクタ
Benchmark::Benchmark()
{
	QueryPerformanceFrequency(&ticksPerSecond);
	QueryPerformanceCounter(&startTicks);
	QueryPerformanceCounter(&currentTicks);
}

// 開始位置
void Benchmark::Begin()
{
	QueryPerformanceCounter(&startTicks);
}

// 終了位置
float Benchmark::End()
{
	QueryPerformanceCounter(&currentTicks);
	return static_cast<float>(currentTicks.QuadPart - startTicks.QuadPart) /
		static_cast<float>(ticksPerSecond.QuadPart);
}
