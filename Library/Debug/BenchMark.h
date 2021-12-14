#pragma once

#include <windows.h>
#include <crtdbg.h>

// ベンチマーククラス
class Benchmark
{
public:
	// コンストラクタ
	Benchmark();

	// デストラクタ
	~Benchmark() = default;

	// 開始位置
	void Begin();

	// 終了位置
	float End();

private:
	LARGE_INTEGER ticksPerSecond	= {};
	LARGE_INTEGER startTicks		= {};
	LARGE_INTEGER currentTicks		= {};
};
