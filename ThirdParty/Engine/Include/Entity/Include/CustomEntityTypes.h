#pragma once

#include <bitset>
#include <GrowingArray.h>
#include <StaticArray.h>

#define MAX_NUMBER_OF_COMPONENTS 32

namespace Magma
{
	class BaseComponent;

	typedef unsigned int Entity;

	typedef CU::GrowingArray<BaseComponent*> ComponentArray;
	typedef CU::StaticArray<int, MAX_NUMBER_OF_COMPONENTS> EntityComponentArray;
}