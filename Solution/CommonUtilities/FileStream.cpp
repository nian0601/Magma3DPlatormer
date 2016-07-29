#include "stdafx.h"

#include "CommonHelper.h"
#include "FileStream.h"


namespace CU
{
	FileStream::FileStream(const CU::String<64>& aFilePath, int aFlags)
		: myFlags(aFlags)
	{
		CU::String<3> flags;
		if (aFlags & eFlag::READ)
		{
			DL_ASSERT_EXP((aFlags & eFlag::WRITE) == false, "Cant open FileStream as both READ and WRITE");
			flags += "r";
		}
		if (aFlags & eFlag::WRITE)
		{
			DL_ASSERT_EXP((aFlags & eFlag::READ) == false, "Cant open FileStream as both READ and WRITE");
			flags += "w";
		}
		if (aFlags & eFlag::BINARY)
		{
			flags += "b";
		}

		errno_t result = fopen_s(&myFileHandle, aFilePath.c_str(), flags.c_str());
		DL_ASSERT_EXP(result == 0, CU::Concatenate<256>("Failed to open FileStream: %s", aFilePath.c_str()));
		if (result != 0)
		{
			fclose(myFileHandle);
		}
	}


	FileStream::~FileStream()
	{
		Close();
	}

	void FileStream::Write(int aValue)
	{
		DL_ASSERT_EXP(IsFlagged(eFlag::WRITE), "Cant Write to a Stream not opened with WRITE");
		fwrite(&aValue, sizeof(int), 1, myFileHandle);
	}

	void FileStream::Write(float aValue)
	{
		DL_ASSERT_EXP(IsFlagged(eFlag::WRITE), "Cant Write to a Stream not opened with WRITE");
		fwrite(&aValue, sizeof(float), 1, myFileHandle);
	}

	void FileStream::Write(char aValue)
	{
		DL_ASSERT_EXP(IsFlagged(eFlag::WRITE), "Cant Write to a Stream not opened with WRITE");
		fwrite(&aValue, sizeof(char), 1, myFileHandle);
	}

	void FileStream::Read(int &aValue)
	{
		DL_ASSERT_EXP(IsFlagged(eFlag::READ), "Cant Read from a Stream not opened with READ");
		fread_s(&aValue, sizeof(int), sizeof(int), 1, myFileHandle);
	}

	void FileStream::Read(float &aValue)
	{
		DL_ASSERT_EXP(IsFlagged(eFlag::READ), "Cant Read from a Stream not opened with READ");
		fread_s(&aValue, sizeof(float), sizeof(float), 1, myFileHandle);
	}

	void FileStream::Read(char &aValue)
	{
		DL_ASSERT_EXP(IsFlagged(eFlag::READ), "Cant Read from a Stream not opened with READ");
		fread_s(&aValue, sizeof(char), sizeof(char), 1, myFileHandle);
	}

	void FileStream::Close()
	{
		fclose(myFileHandle);
	}

	bool FileStream::IsFlagged(int aFlag) const
	{
		return (myFlags & aFlag) > 0;
	}

}