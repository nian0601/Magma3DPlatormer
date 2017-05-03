#pragma once

namespace Magma
{
	struct Event
	{
		Event(unsigned int aID)
			: myType(aID)
		{}

		const unsigned int myType;
	};
}