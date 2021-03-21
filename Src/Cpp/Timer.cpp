#include"Timer.h"

Timer::Timer() :
	pause(false),
	deltaTime(0),
	secondsPerCount(0),
	startTiming(0),
	currentTiming(0),
	previousTiming(0),
	pauseTiming(0),
	pauseTime(0)
{
	__int64 counts_per_second;
	QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_second);
	secondsPerCount = 1.0 / (double)counts_per_second;

}

void Timer::Tick()
{
	if (pause) {
		deltaTime = 0;
		return;
	}
	else {
		__int64 current_timing;
		QueryPerformanceCounter((LARGE_INTEGER*)&current_timing);
		currentTiming = current_timing;

		// Time difference between this frame and the previous.
		deltaTime = (currentTiming - previousTiming) * secondsPerCount;

		// Prepare for next frame.
		previousTiming = currentTiming;

		if (deltaTime < 0.0)
		{
			deltaTime = 0.0;
		}
	}
}

float Timer::DeltaTime()
{
	return (float)deltaTime;
}

float Timer::TotalTime()
{
	if (pause)
	{
		return (float)(((pauseTiming - pauseTime) - startTiming) * secondsPerCount);
	}

	else
	{
		return (float)(((currentTiming - pauseTime) - startTiming) * secondsPerCount);
	}
}

void Timer::Pause()
{
	if (!pause) {
		pause = true;
		__int64 current_timing;
		QueryPerformanceCounter((LARGE_INTEGER*)&current_timing);

		pauseTiming = current_timing;
	}
}

void Timer::Start()
{
	__int64 start_timing;
	QueryPerformanceCounter((LARGE_INTEGER*)&start_timing);

	if (pause)
	{
		pauseTime += (start_timing - pauseTiming);

		previousTiming = start_timing;
		pauseTiming = 0;
		pause = false;
	}
}

void Timer::Reset()
{
	__int64 current_timing;
	QueryPerformanceCounter((LARGE_INTEGER*)&current_timing);

	startTiming = current_timing;
	previousTiming = current_timing;
	pauseTiming = 0;
	pauseTime = 0;	// 涉及到多次Reset的话需要将其归0
	pause = false;
}
