#pragma once
#include"windows.h"

class Timer {

private:
	//计时器是否暂停
	bool pause;

	//帧时间
	double deltaTime;
	//计数器每计数时间
	double secondsPerCount;

	//计时开始时间点
	__int64 startTiming;
	//当前帧时间点
	__int64 currentTiming;
	//上一帧时间点
	__int64 previousTiming;
	//暂停时时间点
	__int64 pauseTiming;

	//暂停经过的时间
	__int64 pauseTime;


public:
	Timer();

	void Tick();
	float DeltaTime();
	float TotalTime();
	void Pause();
	void Start();
	void Reset();


};