#pragma once

namespace Magma
{
	template <typename BaseType>
	class TypeID
	{
	public:
		template <typename T>
		static unsigned int GetID()
		{
			static const unsigned int id = myNextID++;
			return id;
		}

	private:
		static unsigned int myNextID;
	};

	template <typename BaseType>
	unsigned int TypeID<BaseType>::myNextID = 0;
}