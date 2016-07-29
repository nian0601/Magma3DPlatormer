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

		FileStream(const CU::String<64>& aFilePath, int aFlags);
		~FileStream();

		void Write(int aValue);
		void Write(float aValue);
		void Write(char aValue);
		template<unsigned int T>
		void Write(const CU::String<T>& aValue);

		void Read(int &aValue);
		void Read(float &aValue);
		void Read(char &aValue);
		template<unsigned int T>
		void Read(CU::String<T>& aValue);

		void Close();

	private:
		bool IsFlagged(int aFlag) const;

		FILE* myFileHandle;
		int myFlags;
	};

	template<unsigned int T>
	void FileStream::Write(const CU::String<T>& aValue)
	{
		DL_ASSERT_EXP(IsFlagged(eFlag::WRITE), "Cant Write to a Stream not opened with WRITE");
		Write(aValue.Size());
		fwrite(&aValue, sizeof(aValue), 1, myFileHandle);

	}

	template<unsigned int T>
	void FileStream::Read(CU::String<T>& aValue)
	{
		DL_ASSERT_EXP(IsFlagged(eFlag::READ), "Cant Read from a Stream not opened with READ");
		int lenght = 0;
		Read(lenght);
		DL_ASSERT_EXP(T >= lenght, "Supplied a string thats to small to FileStream::Read");

		fread_s(&aValue, sizeof(aValue), sizeof(aValue), 1, myFileHandle);
	}
}