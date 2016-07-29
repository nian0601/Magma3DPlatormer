#pragma once

#include "Murmur.h"
#include "CUString.h"

template<unsigned int MaxSize = 80>
unsigned int Murmur(const CU::String<MaxSize>& aValue)
{
	unsigned int result = 0;
	MurmurHash3_x86_32(aValue.c_str(), aValue.Size(), 2654435761, &result);

	return result;
}

inline unsigned int Murmur(int aValue)
{
	unsigned int result = 0;
	MurmurHash3_x86_32(&aValue, 1, 2654435761, &result);

	return result;
}

inline unsigned int Murmur(unsigned int aValue)
{
	unsigned int result = 0;
	MurmurHash3_x86_32(&aValue, 1, 2654435761, &result);

	return result;
}