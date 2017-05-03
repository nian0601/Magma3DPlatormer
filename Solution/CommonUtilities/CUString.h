#pragma once

#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include "Macros.h"
#include "Murmur.h"

#define CU_STRING_START_SIZE 20
#define CU_STRING_MAX_NUMBER_CHARACTERS 30

namespace CU
{
	class String
	{
		friend class FileStream;
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

		void Resize(int aNewSize);
		unsigned int GetHash() const;

		void Clear();

	private:
		void UpdateHash();

		int myCurrentSize;
		int myMaxSize;
		unsigned int myHash;
		char* myData;
	};

	inline String::String()
		: myCurrentSize(0)
		, myMaxSize(CU_STRING_START_SIZE)
		, myData(new char[CU_STRING_START_SIZE])
	{
		myHash = 0;
	}

	inline String::~String()
	{
		SAFE_ARRAY_DELETE(myData);
	}

	inline String::String(const String &aString)
		: myCurrentSize(0)
		, myMaxSize(CU_STRING_START_SIZE)
		, myData(new char[CU_STRING_START_SIZE])
	{
		*this = aString;
	}

	inline String::String(const char* aString)
		: myCurrentSize(0)
		, myMaxSize(CU_STRING_START_SIZE)
		, myData(new char[CU_STRING_START_SIZE])
	{
		*this = aString;
	}

	inline String::String(const char aChar)
		: myCurrentSize(0)
		, myMaxSize(CU_STRING_START_SIZE)
		, myData(new char[CU_STRING_START_SIZE])
	{
		myHash = 0;
		myData[myCurrentSize] = aChar;
		++myCurrentSize;
		UpdateHash();
	}

	inline String& String::operator+=(const String &aString)
	{
		/*assert(myCurrentSize < MaxSize && "[String] Tried to add a String to an already full string.");
		assert(myCurrentSize + aString.Size() < MaxSize && "[String]: Tried to add to an string that would result in an overflow.");
		*/
		if (myCurrentSize + aString.Size()+1 >= myMaxSize)
			Resize(myCurrentSize + aString.Size()+1);

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

	inline String& String::operator+=(const float aFloat)
	{
		//assert(myCurrentSize < MaxSize && "[String] Tried to add a Float to an already full string.");

		if (myCurrentSize >= myMaxSize)
			Resize(myCurrentSize * 2);

		char buffer[CU_STRING_MAX_NUMBER_CHARACTERS];
		_snprintf_s(buffer, CU_STRING_MAX_NUMBER_CHARACTERS, "%f", aFloat);

		String newString(buffer);
		*this += newString;

		return *this;
	}

	inline String& String::operator+=(const double aDouble)
	{
		//assert(myCurrentSize < MaxSize && "[String] Tried to add a Double to an already full string.");


		if (myCurrentSize >= myMaxSize)
			Resize(myCurrentSize * 2);

		char buffer[CU_STRING_MAX_NUMBER_CHARACTERS];
		_snprintf_s(buffer, CU_STRING_MAX_NUMBER_CHARACTERS, "%f", aDouble);

		String newString(buffer);
		*this += newString;

		return *this;
	}

	inline String& String::operator+=(const int aInt)
	{
		//assert(myCurrentSize < MaxSize && "[String] Tried to add a Int to an already full string.");

		if (myCurrentSize >= myMaxSize)
			Resize(myCurrentSize * 2);

		char buffer[CU_STRING_MAX_NUMBER_CHARACTERS];
		_snprintf_s(buffer, CU_STRING_MAX_NUMBER_CHARACTERS, "%i", aInt);
		
		String newString(buffer);
		*this += newString;

		return *this;
	}

	inline String& String::operator+=(const char* aString)
	{
		//assert(myCurrentSize < MaxSize && "[String] Tried to add a Char* to an already full string.");

		if (myCurrentSize >= myMaxSize)
			Resize(myCurrentSize * 2);

		String newString(aString);
		*this += newString;

		return *this;
	}

	inline String& String::operator+=(const char aChar)
	{
		//assert(myCurrentSize < MaxSize && "[String] Tried to add a Char to an already full string.");

		if (myCurrentSize >= myMaxSize)
			Resize(myCurrentSize * 2);

		String newString(aChar);
		*this += newString;

		return *this;
	}


	inline bool String::operator==(const String &aString) const
	{
		return myHash == aString.myHash;

		/*if (aString.Size() != Size()) return false;

		for (int i = 0; i < aString.Size(); ++i)
		if (aString[i] != myData[i]) return false;

		return true;*/
	}

	inline void String::operator=(const String &aString)
	{
		int targetLenght = aString.Size();
		if (myMaxSize <= targetLenght+1)
			Resize(targetLenght+1);
		//assert(aString.Size() < MaxSize && "[String] Tried to assign a larger string to a smaller one.");

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

	inline void String::operator=(const char* aString)
	{
		int targetLenght = static_cast<int>(strlen(aString));
		if (myMaxSize <= targetLenght)
			Resize(targetLenght);
		//assert(strlen(aString) < MaxSize && "[String]: Tried to assing a larget string to a smaller one.");

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


	inline int String::Find(const char* aPattern, const int aStartIndex) const
	{
		String pattern(aPattern);

		return Find(pattern, aStartIndex);
	}

	inline int String::Find(const String &aPattern, const int aStartIndex) const
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

	inline int String::RFind(const char* aPattern) const
	{
		String pattern(aPattern);

		return RFind(pattern);
	}

	inline int String::RFind(const String &aPattern) const
	{
		int lenghtOfText = myCurrentSize;
		int lenghtOfPattern = aPattern.myCurrentSize;

		int jumpTable[256];
		for (int x = 0; x < 256; ++x)
		{
			jumpTable[x] = NotFound;
		}


		for (int x = lenghtOfPattern - 1; x > 0; --x)
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


	inline String String::SubStr(const int aStart, const int aEnd) const
	{
		assert(aStart < myMaxSize && "[String]: Tried to create a SubString starting at an Index greater than the lenght of the main-string.");
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

	inline const char* String::c_str() const
	{
		return myData;
	}

	inline int String::Size() const
	{
		return myCurrentSize - 1;
	}

	inline bool String::Empty() const
	{
		return myCurrentSize == 0;
	}

	inline const char& String::operator[](const int aIndex) const
	{
		assert(aIndex < myCurrentSize + 1 && "[String]: Tried to use Subscript with an index greater than the lenght.");
		assert(aIndex >= 0 && "[Sting]: Tried to use Subscript with an index smaller than 0");

		return myData[aIndex];
	}

	inline char& String::operator[](const int aIndex)
	{
		assert(aIndex < myCurrentSize + 1 && "[String]: Tried to use Subscript with an index greater than the lenght.");
		assert(aIndex >= 0 && "[Sting]: Tried to use Subscript with an index smaller than 0");

		return myData[aIndex];
	}

	inline void String::Resize(int aNewSize)
	{
		assert(aNewSize > 0 && "Can't make String smaller than 1.");

		myMaxSize = aNewSize+1;
		char* newData = new char[myMaxSize];
		//memcpy(newData, myData, sizeof(char) * myCurrentSize);
		for (int i = 0; i < myCurrentSize; ++i)
		{
			newData[i] = myData[i];
		}

		newData[myCurrentSize] = NullTermination;

		delete[] myData;
		myData = newData;
	}

	inline unsigned int String::GetHash() const
	{
		return myHash;
	}

	inline void String::Clear()
	{
		for (int i = 0; i < myCurrentSize; ++i)
			myData[i] = NullTermination;

		myCurrentSize = 0;
	}

	inline void String::UpdateHash()
	{
		MurmurHash3_x86_32(myData, myCurrentSize, 1000, &myHash);
	}

	//
	//
	//

	inline String operator+(const String &aFirst, const String &aSecond)
	{
		String newString(aFirst);
		newString += aSecond;

		return newString;
	}

	inline String operator+(const String &aString, const float aFloat)
	{
		String newString(aString);
		newString += aFloat;

		return newString;
	}

	inline String operator+(const String &aString, const double aDouble)
	{
		String newString(aString);
		newString += aDouble;

		return newString;
	}

	inline String operator+(const String &aString, const int aInt)
	{
		String newString(aString);
		newString += aInt;

		return newString;
	}

	inline String operator+(const String &aFirst, const char* aSecond)
	{
		String newString(aFirst);
		newString += aSecond;

		return newString;
	}

	inline String operator+(const String &aString, const char aChar)
	{
		String newString(aString);
		newString += aChar;

		return newString;
	}

	//

	inline String operator+(const float aFloat, const String &aString)
	{
		String newString(aString);
		newString += aFloat;

		return newString;
	}

	inline String operator+(const double aDouble, const String &aString)
	{
		String newString(aString);
		newString += aDouble;

		return newString;
	}

	inline String operator+(const int aInt, const String &aString)
	{
		String newString(aString);
		newString += aInt;

		return newString;
	}

	inline String operator+(const char* aSecond, const String &aFirst)
	{
		String newString(aFirst);
		newString += aSecond;

		return newString;
	}

	inline String operator+(const char aChar, const String &aString)
	{
		String newString(aString);
		newString += aChar;

		return newString;
	}
}