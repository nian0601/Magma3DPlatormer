#include "stdafx.h"

#include "TimerManager.h"
#include <Windows.h>

CU::TimerManager::TimerManager()
{
	LARGE_INTEGER largeInteger;
	QueryPerformanceFrequency(&largeInteger);
	myFrequency = largeInteger.QuadPart;

	QueryPerformanceCounter(&largeInteger);
	myCurrentTime = largeInteger.QuadPart * 1000000 / myFrequency;
	myLastTime = myCurrentTime;
}

void CU::TimerManager::Update()
{
	myLastTime = myCurrentTime;
	
	LARGE_INTEGER largeInteger;
	QueryPerformanceCounter(&largeInteger);
	myCurrentTime = largeInteger.QuadPart * 1000000 / myFrequency;
}

void CU::TimerManager::CapFrameRate(float aFrameRate)
{
	float capTime = 1.f / aFrameRate;

	float waitTime = 0;

	while (waitTime < capTime)
	{
		float frameTime = GetFrameTime();
		waitTime = frameTime;
		if (waitTime + 0.002f < capTime)
		{
			//Yield(1);
		}
	}
}

float CU::TimerManager::GetFrameTime() const
{
	return static_cast<float>(myCurrentTime - myLastTime) / 1000000.f;
}

unsigned long long CU::TimerManager::GetCurrentTime() const
{
	return myCurrentTime;
}