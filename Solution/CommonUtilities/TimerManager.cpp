#include "stdafx.h"

#include <assert.h>
#include "TimeOwn.h"
#include "TimerManager.h"
#include <Windows.h>

CU::TimerManager::TimerManager()
{
	LARGE_INTEGER largeInteger;
	QueryPerformanceFrequency(&largeInteger);
	myFrequency = largeInteger.QuadPart;

	QueryPerformanceCounter(&largeInteger);
	myLastTime = largeInteger.QuadPart * 1000000 / myFrequency;
}

void CU::TimerManager::Update()
{
	TimeUnit time = GetTime();
	myMasterTimer.AddTime(time);
	myLastTime += time;
}

const CU::Timer& CU::TimerManager::GetMasterTimer() const
{
	return myMasterTimer;
}

CU::TimeUnit CU::TimerManager::GetTime()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	return (current.QuadPart * 1000000 / myFrequency) - myLastTime;
}

void CU::TimerManager::CapFrameRate(float aFrameRate)
{
	float capTime = 1.f / aFrameRate;

	float waitTime = 0;

	while (waitTime < capTime)
	{
		float frameTime = float(GetTime());
		frameTime /= 1000000.f;
		waitTime = frameTime;
		if (waitTime + 0.002f < capTime)
		{
			//Yield(1);
		}
	}
}