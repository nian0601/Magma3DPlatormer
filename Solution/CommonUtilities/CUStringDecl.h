#pragma once

#include <cassert>
#include <stdlib.h>
#include "Macros.h"
#include "Murmur.h"

namespace CU
{
	template<unsigned int MaxSize = 30>
	class String
	{
	public:
		String();
		~String();
		String(const String &aString);
		String(const char* aString);
		String(const char aString);

		String& operator+=(const String &aString);
		String& operator+=(const float aFloat);
		String& operator+=(const double aDouble);
		String& operator+=(const int aInt);
		String& operator+=(const char* aString);
		String& operator+=(const char aString);

		bool operator==(const String &aString) const;
		void operator=(const String &aString);
		void operator=(const char* aString);

		int Find(const char* aPattern, const int aStartIndex = 0) const;
		int Find(const String &aPattern, const int aStartIndex = 0) const;
		int RFind(const char* aPattern) const;
		int RFind(const String &aPattern) const;
		static const int NotFound = -1;
		static const unsigned char NullTermination = 0;

		String SubStr(const int aStart, const int aEnd) const;
		const char* c_str() const;

		int Size() const;
		bool Empty() const;

		const char& operator[](const int aIndex) const;
		char& operator[](const int aIndex);

	private:
		void UpdateHash();
		int myCurrentSize;
		unsigned int myHash;
		char myData[MaxSize];
	};

	template<unsigned int MaxSize = 30>
	String<MaxSize>::String()
	{
		myCurrentSize = 0;
		myHash = 0;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>::~String()
	{

	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>::String(const String &aString)
	{
		*this = aString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>::String(const char* aString)
	{
		*this = aString;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>::String(const char aChar)
	{
		myCurrentSize = 0;
		myData[myCurrentSize] = aChar;
		++myCurrentSize;
		UpdateHash();
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>& String<MaxSize>::operator+=(const String &aString)
	{
		assert(myCurrentSize < MaxSize && "[String] Tried to add a String to an already full string.");
		assert(myCurrentSize + aString.Size() < MaxSize && "[String]: Tried to add to an string that would result in an overflow.");

		for (int i = 0; i < aString.myCurrentSize; ++i)
		{
			myData[myCurrentSize] = aString[i];
			++myCurrentSize;
		}

		if (myData[myCurrentSize] != NullTermination)
		{
			myData[myCurrentSize] = NullTermination;
		}

		UpdateHash();

		return *this;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>& String<MaxSize>::operator+=(const float aFloat)
	{
		assert(myCurrentSize < MaxSize && "[String] Tried to add a Float to an already full string.");

		char buffer[MaxSize];
		_snprintf_s(buffer, MaxSize, "%f", aFloat);

		String newString(buffer);

		*this += newString;

		return *this;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>& String<MaxSize>::operator+=(const double aDouble)
	{
		assert(myCurrentSize < MaxSize && "[String] Tried to add a Double to an already full string.");

		char buffer[MaxSize];
		_snprintf_s(buffer, MaxSize, "%f", aDouble);

		String newString(buffer);

		*this += newString;

		return *this;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>& String<MaxSize>::operator+=(const int aInt)
	{
		assert(myCurrentSize < MaxSize && "[String] Tried to add a Int to an already full string.");

		char buffer[10];
		_itoa_s(aInt, buffer, 10);

		String newString(buffer);

		*this += newString;

		return *this;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>& String<MaxSize>::operator+=(const char* aString)
	{
		assert(myCurrentSize < MaxSize && "[String] Tried to add a Char* to an already full string.");

		String newString(aString);

		*this += newString;

		return *this;
	}

	template<unsigned int MaxSize = 30>
	String<MaxSize>& String<MaxSize>::operator+=(const char aChar)
	{
		assert(myCurrentSize < MaxSize && "[String] Tried to add a Char to an already full string.");

		String newString(aChar);

		*this += newString;

		return *this;
	}


	template<unsigned int MaxSize = 30>
	bool String<MaxSize>::operator==(const String &aString) const
	{
		return myHash == aString.myHash;

		/*if (aString.Size() != Size()) return false;

		for (int i = 0; i < aString.Size(); ++i)
		if (aString[i] != myData[i]) return false;

		return true;*/
	}

	template<unsigned int MaxSize = 30>
	void String<MaxSize>::operator=(const String &aString)
	{
		assert(aString.Size() < MaxSize && "[String] Tried to assign a larger string to a smaller one.");

		myCurrentSize = 0;
		for (int i = 0; i < aString.myCurrentSize; ++i)
		{
			myData[myCurrentSize] = aString[i];
			++myCurrentSize;
		}

		if (myData[myCurrentSize] != NullTermination)
		{
			myData[myCurrentSize] = NullTermination;
		}

		UpdateHash();
	}

	template<unsigned int MaxSize = 30>
	void String<MaxSize>::operator=(const char* aString)
	{
		assert(strlen(aString) < MaxSize && "[String]: Tried to assing a larget string to a smaller one.");

		myCurrentSize = 0;
		for (unsigned int i = 0; i < strlen(aString); ++i)
		{
			myData[myCurrentSize] = aString[i];
			++myCurrentSize;
		}

		if (myData[myCurrentSize] != NullTermination)
		{
			myData[myCurrentSize] = NullTermination;
		}

		UpdateHash();
	}


	template<unsigned int MaxSize = 30>
	int String<MaxSize>::Find(const char* aPattern, const int aStartIndex = 0) const
	{
		String pattern(aPattern);

		return Find(pattern, aStartIndex);
	}

	template<unsigned int MaxSize = 30>
	int String<MaxSize>::Find(const String &aPattern, const int aStartIndex = 0) const
	{
		int lenghtOfText = myCurrentSize;
		int lenghtOfPattern = aPattern.myCurrentSize;

		int jumpTable[256];
		for (int i = 0; i < 256; ++i)
		{
			jumpTable[i] = NotFound;
		}

		for (int k = 0; k < lenghtOfPattern - 1; ++k)
		{
			jumpTable[aPattern[k]] = lenghtOfPattern - k - 1;
		}
		

		if (lenghtOfPattern > lenghtOfText) return NotFound;

		int k = lenghtOfPattern - 1 + aStartIndex;

		while (k < lenghtOfText)
		{
			int j = lenghtOfPattern - 1;
			int i = k;
			while (j >= 0 && myData[i] == aPattern[j])
			{
				--j;
				--i;
			}
			if (j == -1)
				return i + 1;


			int jumpValue = jumpTable[myData[k]];
			if (jumpValue == NotFound)
			{
				k += lenghtOfPattern;
			}
			else
			{
				k += jumpValue;
			}
		}


		return NotFound;
	}

	template<unsigned int MaxSize = 30>
	int String<MaxSize>::RFind(const char* aPattern) const
	{
		String pattern(aPattern);

		return RFind(pattern);
	}

	template<unsigned int MaxSize = 30>
	int String<MaxSize>::RFind(const String &aPattern) const
	{
		int lenghtOfText = myCurrentSize;
		int lenghtOfPattern = aPattern.myCurrentSize;

		int jumpTable[256];
		for (int x = 0; x < 256; ++x)
		{
			jumpTable[x] = NotFound;
		}


		for (int x = lenghtOfPattern-1; x > 0; --x)
		{
			jumpTable[aPattern[x]] = x;
		}

		if (lenghtOfPattern > lenghtOfText) return NotFound;

		int k = myCurrentSize - 1;

		while (k >= 0)
		{
			int j = 0;
			int i = k;
			while (j <= lenghtOfPattern /*- 1*/ && myData[i] == aPattern[j])
			{
				++j;
				++i;
			}
			if (j == lenghtOfPattern)
				return i - lenghtOfPattern;


			int jumpValue = jumpTable[myData[k]];
			if (jumpValue == NotFound)
			{
				k -= lenghtOfPattern;
			}
			else
			{
				k -= jumpValue;
			}
		}


		return NotFound;
	}


	template<unsigned int MaxSize = 30>
	String<MaxSize> String<MaxSize>::SubStr(const int aStart, const int aEnd) const
	{
		assert(aStart < MaxSize && "[String]: Tried to create a SubString starting at an Index greater than the lenght of the main-string.");
		assert(aStart >= 0 && "[String]: Tried to create a SubString starting at an Index greater than the lenght of the main-string.");
		assert(aEnd <= Size() && "[String]: Tried to create a SubString thats longer than the main-string.");

		String newString;
		for (int i = aStart; i <= aEnd; ++i)
		{
			newString += static_cast<char>(operator[](i));
		}


		if (newString[newString.myCurrentSize] != NullTermination)
		{
			newString[newString.myCurrentSize] = NullTermination;
		}
		
		return newString;
	}

	template<unsigned int MaxSize = 30>
	const char* String<MaxSize>::c_str() const
	{
		return myData;
	}

	template<unsigned int MaxSize = 30>
	int String<MaxSize>::Size() const
	{
		return myCurrentSize - 1;
	}

	template<unsigned int MaxSize = 30>
	bool String<MaxSize>::Empty() const
	{
		return myCurrentSize == 0;
	}

	template<unsigned int MaxSize = 30>
	const char& String<MaxSize>::operator[](const int aIndex) const
	{
		assert(aIndex < myCurrentSize + 1 && "[String]: Tried to use Subscript with an index greater than the lenght.");
		assert(aIndex >= 0 && "[Sting]: Tried to use Subscript with an index smaller than 0");

		return myData[aIndex];
	}

	template<unsigned int MaxSize = 30>
	char& String<MaxSize>::operator[](const int aIndex)
	{
		assert(aIndex < myCurrentSize + 1 && "[String]: Tried to use Subscript with an index greater than the lenght.");
		assert(aIndex >= 0 && "[Sting]: Tried to use Subscript with an index smaller than 0");

		return myData[aIndex];
	}

	template<unsigned int MaxSize = 30>
	void String<MaxSize>::UpdateHash()
	{
		MurmurHash3_x86_32(myData, myCurrentSize, 1000, &myHash);

		//myHash = Murmur::Hash(myData);
	}
}