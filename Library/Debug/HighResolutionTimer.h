#pragma once

#include <windows.h>

class HighResolutionTimer
{
public:
	// コンストラクタ
	HighResolutionTimer();

	// デストラクタ
	~HighResolutionTimer() = default;

	//
	float TimeStamp() const;

	float TimeInterval() const;

	void Reset();

	void Start();

	void Stop();

	void Tick();

private:
	double secondsPerCount	= 0.0;
	double deltaTime		= 0.0;

	LONGLONG baseTime		= 0;
	LONGLONG pausedTime		= 0;
	LONGLONG stopTime		= 0;
	LONGLONG lastTime		= 0;
	LONGLONG thisTime		= 0;

	bool stopped			= false;
};
