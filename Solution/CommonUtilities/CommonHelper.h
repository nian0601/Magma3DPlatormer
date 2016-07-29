#pragma once
#include <algorithm>
#include "CUString.h"
#include "DL_Debug.h"
#include "Matrix.h"
#include "Murmur.h"
#include "Vector.h"
#include <ShlObj.h>
#include <fstream>
#include <streambuf>

namespace CU
{
	const CU::String<80> group("/Distortion GamesMagma/");
	const CU::String<80> game("");

	static bool canSave;
	
	inline bool dirExists(const CU::String<80>& dirName_in)
	{
		DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;  //something is wrong with your path!

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;   // this is a directory!

		return false;    // this is not a directory!
	}

	inline void BuildFoldersInPath(const CU::String<80>& aPath)
	{
		unsigned int slashIndex = aPath.Find("/");

		while (slashIndex != aPath.NotFound)
		{
			CU::String<80> folder = aPath.SubStr(0, slashIndex);
			CreateDirectory(folder.c_str(), NULL);
			slashIndex = aPath.Find("/", slashIndex + 1);
		}
	}

	inline bool FileExists(const CU::String<80>& name) {
		std::ifstream f(name.c_str());
		if (f.good()) {
			f.close();
			return true;
		}
		f.close();
		return false;
	}

	inline void CreateMyDocumentsFolderPath()
	{
		char documents[MAX_PATH];
		HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documents);
		if (hr != S_OK)
		{
			canSave = false;
			DL_ASSERT("Can't get document folder!");
		}

		CU::String<80> directory(documents);
		directory += group;
		directory += game;
		if (dirExists(directory) == false)
		{
			BuildFoldersInPath(directory);
		}
	}

	inline void CreateFileIfNotExists(const CU::String<80>& aFilePath)
	{
		if (FileExists(aFilePath) == false)
		{
			std::ofstream outFile(aFilePath.c_str());
			//outFile << "apa";
			outFile.close();
		}
	}

	inline CU::String<80> GetMyDocumentFolderPath()
	{
		char documents[MAX_PATH];
		HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documents);
		if (hr != S_OK)
		{
			DL_ASSERT("Can't get document folder!");
		}

		CU::String<80> output(documents);
		output += group + game;
		return output;
	}

	inline float Clip(float aNumber, float aLower, float aUpper)
	{
		return fmax(aLower, fmin(aNumber, aUpper));
	}

	inline int ClipInt(int aNumber, int aLower, int aUpper)
	{
		return max(aLower, min(aNumber, aUpper));
	}

	template<unsigned int MaxSize = 80>
	inline CU::String<80> GetSubString(const CU::String<MaxSize>& aStringToReadFrom, char aCharToFind, bool aReadAfterChar)
	{
		CU::String<MaxSize> toReturn;
		if (aReadAfterChar == false)
		{
			toReturn = aStringToReadFrom.SubStr(0, aStringToReadFrom.Find(aCharToFind)-1);
		}
		else if (aReadAfterChar == true)
		{
			toReturn = aStringToReadFrom.SubStr(aStringToReadFrom.Find(aCharToFind), aStringToReadFrom.Size());
		}

		return toReturn;
	}

	template<unsigned int MaxSize = 80>
	inline CU::String<MaxSize> GetSubString(const CU::String<MaxSize>& aStringToReadFrom, const CU::String<MaxSize>& aWordToFind, bool aReadAfterChar
		, int someCharsToSkip = 0)
	{
		int index = aStringToReadFrom.RFind(aWordToFind);
		if (index != aStringToReadFrom.NotFound)
		{
			if (aReadAfterChar == true)
			{
				return aStringToReadFrom.SubStr(0, index);
			}

			return aStringToReadFrom.SubStr(index + someCharsToSkip, aStringToReadFrom.Size());
		}

		return aStringToReadFrom;
	}

	//If OptionalExtension is blank, the outputstring will have the same extension as the input string
	//OptionalExtension needs to be entered without a period, "xml", NOT ".xml"
	inline CU::String<80> GetGeneratedDataFolderFilePath(const CU::String<80>& aFilePath, const CU::String<80>& anOptionalNewExtension = "")
	{
		CU::String<80> pathWithoutData = aFilePath.SubStr(5, aFilePath.Size());

		if (anOptionalNewExtension.Empty() == false)
		{
			int extensionIndex = pathWithoutData.RFind(".");
			pathWithoutData = pathWithoutData.SubStr(0, extensionIndex + 1);

			pathWithoutData += anOptionalNewExtension;
		}

		CU::String<80> generatedDataFilePath = "GeneratedData/";
		generatedDataFilePath += pathWithoutData;

		return generatedDataFilePath;
	}

	//If OptionalExtension is blank, the outputstring will have the same extension as the input string
	//OptionalExtension needs to be entered without a period, "xml", NOT ".xml"
	inline CU::String<80> GetRealDataFolderFilePath(const CU::String<80>& aFilePath, const CU::String<80>& anOptionalNewExtension = "")
	{
		CU::String<80> pathWithoutData = aFilePath.SubStr(14, aFilePath.Size());

		if (anOptionalNewExtension.Empty() == false)
		{
			int extensionIndex = pathWithoutData.RFind(".");
			pathWithoutData = pathWithoutData.SubStr(0, extensionIndex + 1);

			pathWithoutData += anOptionalNewExtension;
		}

		CU::String<80> generatedDataFilePath = "Data/";
		generatedDataFilePath += pathWithoutData;

		return generatedDataFilePath;
	}
	
	template<unsigned int MaxSize = 30>
	inline CU::String<MaxSize> Concatenate(const char* aFormattedString, ...)
	{
		char buffer[1024];
		va_list args;
		va_start(args, aFormattedString);
		if (vsprintf_s(buffer, aFormattedString, args) < 0)
		{
			perror(buffer);
		}
		va_end(args);

		return buffer;
	}

	inline void TrimWhiteSpacesAtBeginAndEnd(CU::String<80>& aString)
	{
		if (aString.Empty() == true)
		{
			return;
		}

		int begin = 0;
		while (aString[begin] == ' ')
		{
			++begin;
		}

		int end = aString.Size();
		if (end > begin)
		{
			while (aString[end-1] == ' ')
			{
				--end;
			}
		}
		if (begin != 0 || end != aString.Size())
		{
			aString = aString.SubStr(begin, end);
		}
	}


	inline Matrix44<float> GetOrientation(Matrix44<float>& aMatrixToRotate, const Vector3<float>& aRotationInDegrees)
	{
		//to radians:
		Vector3<float> rotation(aRotationInDegrees);
		rotation *= 0.0174532925f;
		
		aMatrixToRotate = Matrix44<float>::CreateRotateAroundZ(rotation.z) * aMatrixToRotate;
		aMatrixToRotate = Matrix44<float>::CreateRotateAroundY(rotation.y) * aMatrixToRotate;
		aMatrixToRotate = Matrix44<float>::CreateRotateAroundX(rotation.x) * aMatrixToRotate;

		return aMatrixToRotate;
	}

	inline bool IsValidTextureSize(UINT aValue)
	{
		return aValue == 2 || aValue == 4 || aValue == 8 || aValue == 16 || aValue == 32 || aValue == 64 || aValue == 128
			|| aValue == 256 || aValue == 512 || aValue == 1024 || aValue == 2048 || aValue == 4096 || aValue == 8192
			|| aValue == 1024 * 6;
	}

	inline CU::String<64> GetFileNameFromFilePath(const CU::String<64>& aFilePath)
	{
		int index = aFilePath.RFind("/") + 1;

		CU::String<64> result = aFilePath.SubStr(index, aFilePath.Size());
		return result;
	}
}