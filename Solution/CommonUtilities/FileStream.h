#pragma once

#include "CUString.h"
#include <cstdio>

namespace CU
{
	class FileStream
	{
	public:
		enum eFlag
		{
			READ = 1,
			WRITE = 2,
			BINARY = 4,
		};

		FileStream(const CU::String& aFilePath, int aFlags);
		~FileStream();

		void Write(int aValue);
		void Write(float aValue);
		void Write(char aValue);
		void Write(const CU::String& aValue);

		void Read(int &aValue);
		void Read(float &aValue);
		void Read(char &aValue);
		void Read(CU::String& aValue);

		void Close();

	private:
		bool IsFlagged(int aFlag) const;

		FILE* myFileHandle;
		int myFlags;
	};
}