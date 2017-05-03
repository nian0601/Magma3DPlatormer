#pragma once

namespace CU
{
	class TimerManager
	{
	public:
		TimerManager();

		void Update();
		void CapFrameRate(float aFrameRate);

		float GetFrameTime() const;
		unsigned long long GetCurrentTime() const;

	private:
		unsigned long long myLastTime;
		unsigned long long myCurrentTime;

		unsigned long long myFrequency;

	};
}