#pragma once

#include "CUStringDecl.h"

namespace CU
{
	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const String<MaxSize> &aFirst, const String<MaxSize> &aSecond)
	{
		String<MaxSize> newString(aFirst);
		newString += aSecond;

		return newString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const String<MaxSize> &aString, const float aFloat)
	{
		String<MaxSize> newString(aString);
		newString += aFloat;

		return newString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const String<MaxSize> &aString, const double aDouble)
	{
		String<MaxSize> newString(aString);
		newString += aDouble;

		return newString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const String<MaxSize> &aString, const int aInt)
	{
		String<MaxSize> newString(aString);
		newString += aInt;

		return newString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const String<MaxSize> &aFirst, const char* aSecond)
	{
		String<MaxSize> newString(aFirst);
		newString += aSecond;

		return newString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const String<MaxSize> &aString, const char aChar)
	{
		String<MaxSize> newString(aString);
		newString += aChar;

		return newString;
	}

	//

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const float aFloat, const String<MaxSize> &aString)
	{
		String<MaxSize> newString(aString);
		newString += aFloat;

		return newString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const double aDouble, const String<MaxSize> &aString)
	{
		String<MaxSize> newString(aString);
		newString += aDouble;

		return newString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const int aInt, const String<MaxSize> &aString)
	{
		String<MaxSize> newString(aString);
		newString += aInt;

		return newString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const char* aSecond, const String<MaxSize> &aFirst)
	{
		String<MaxSize> newString(aFirst);
		newString += aSecond;

		return newString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize> operator+(const char aChar, const String<MaxSize> &aString)
	{
		String<MaxSize> newString(aString);
		newString += aChar;

		return newString;
	}
}