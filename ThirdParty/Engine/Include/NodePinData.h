#pragma once

#include <GrowingArray.h>

namespace Magma
{
	struct NodePinData
	{
		enum eType
		{
			NOT_USED,
			INPUT,
			OUTPUT,
		};

		enum eValue
		{
			NOT_USED,
			STRING,
			INT,
			FLOAT,
			BOOL,
			IMPULSE,
			POINTER,
			OTHER_NODE,
		};

		eType myType;
		eValue myValue;

		union
		{
			char* myString;
			int myInt;
			float myFloat;
			bool myBool;
		};

		CU::GrowingArray<unsigned long long> myConnections;
	};
}