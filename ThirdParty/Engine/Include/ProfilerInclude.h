#pragma once

#include "Profiler.h"

#define PROFILE_FUNCTION Magma::ProfilerInclude __profilerInclude__(__FUNCTION__)
#define PROFILE_START(aText) Magma::Profiler::GetInstance()->StartEntry(aText)
#define PROFILE_END Magma::Profiler::GetInstance()->EndEntry()

namespace Magma
{
	class ProfilerInclude
	{
	public:
		ProfilerInclude(const char* aString);
		~ProfilerInclude();
	};
}
