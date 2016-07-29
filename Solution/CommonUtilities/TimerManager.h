#pragma once

#include "CUString.h"
#include "Timer.h"
#include <vector>
#include "CUMap.h"

namespace CU
{
	typedef unsigned int TimerHandle;

	class TimerManager
	{
	public:
		TimerManager();

		const Timer& GetMasterTimer() const;
		void Update();


		void CapFrameRate(float aFrameRate);

	private:
		Timer myMasterTimer;
		TimeUnit GetTime();
		TimeUnit myLastTime;
		unsigned long long myFrequency;
	};
}