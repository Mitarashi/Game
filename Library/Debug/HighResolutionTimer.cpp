#include "HighResolutionTimer.h"

HighResolutionTimer::HighResolutionTimer() : deltaTime(-1.0), pausedTime(0), stopped(false)
{
	LONGLONG countsPerSec = {};
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
	secondsPerCount = 1.0 / static_cast<double>(countsPerSec);

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&thisTime));
	baseTime = thisTime;
	lastTime = thisTime;
}

// Returns the total time elapsed since Reset() was called, NOT counting any
// time when the clock is stopped.
float HighResolutionTimer::TimeStamp() const  // in seconds
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance
	// stop_time - base_time includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:
	//
	//                     |<--paused_time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  base_time       stop_time        start_time     stop_time    this_time

	if (stopped)
	{
		return static_cast<float>(((stopTime - pausedTime) - baseTime) * secondsPerCount);
	}

	// The distance this_time - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract
	// the paused time from this_time:
	//
	//  (thisTime - pausedTime) - baseTime
	//
	//                     |<--pausedTime-->|
	// ----*---------------*-----------------*------------*------> time
	//  baseTime       stopTime        startTime     thisTime
	else
	{
		return static_cast<float>(((thisTime - pausedTime) - baseTime) * secondsPerCount);
	}
}

float HighResolutionTimer::TimeInterval() const  // in seconds
{
	return static_cast<float>(deltaTime);
}

void HighResolutionTimer::Reset() // Call before message loop.
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&thisTime));
	baseTime = thisTime;
	lastTime = thisTime;

	stopTime = 0;
	stopped = false;
}

void HighResolutionTimer::Start() // Call when unpaused.
{
	LONGLONG startTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  baseTime       stopTime        startTime
	if (stopped)
	{
		pausedTime += (startTime - stopTime);
		lastTime = startTime;
		stopTime = 0;
		stopped = false;
	}
}

void HighResolutionTimer::Stop() // Call when paused.
{
	if (!stopped)
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&stopTime));
		stopped = true;
	}
}

void HighResolutionTimer::Tick() // Call every frame.
{
	if (stopped)
	{
		deltaTime = 0.0;
		return;
	}

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&thisTime));
	// Time difference between this frame and the previous.
	deltaTime = (thisTime - lastTime) * secondsPerCount;

	// Prepare for next frame.
	lastTime = thisTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (deltaTime < 0.0)
	{
		deltaTime = 0.0;
	}
}