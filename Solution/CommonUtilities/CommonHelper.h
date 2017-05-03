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
	const CU::String group("/Distortion GamesMagma/");
	const CU::String game("");

	static bool canSave;
	
	inline bool dirExists(const CU::String& dirName_in)
	{
		DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;  //something is wrong with your path!

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;   // this is a directory!

		return false;    // this is not a directory!
	}

	inline void BuildFoldersInPath(const CU::String& aPath)
	{
		unsigned int slashIndex = aPath.Find("/");

		while (slashIndex != aPath.NotFound)
		{
			CU::String folder = aPath.SubStr(0, slashIndex);
			CreateDirectory(folder.c_str(), NULL);
			slashIndex = aPath.Find("/", slashIndex + 1);
		}
	}

	inline bool FileExists(const CU::String& name) {
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

		CU::String directory(documents);
		directory += group;
		directory += game;
		if (dirExists(directory) == false)
		{
			BuildFoldersInPath(directory);
		}
	}

	inline void CreateFileIfNotExists(const CU::String& aFilePath)
	{
		if (FileExists(aFilePath) == false)
		{
			std::ofstream outFile(aFilePath.c_str());
			//outFile << "apa";
			outFile.close();
		}
	}

	inline CU::String GetMyDocumentFolderPath()
	{
		char documents[MAX_PATH];
		HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documents);
		if (hr != S_OK)
		{
			DL_ASSERT("Can't get document folder!");
		}

		CU::String output(documents);
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

	inline CU::String GetSubString(const CU::String& aStringToReadFrom, char aCharToFind, bool aReadAfterChar)
	{
		CU::String toReturn;
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

	inline CU::String GetSubString(const CU::String& aStringToReadFrom, const CU::String& aWordToFind, bool aReadAfterChar
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
	inline CU::String GetGeneratedDataFolderFilePath(const CU::String& aFilePath, const CU::String& anOptionalNewExtension = "")
	{
		CU::String pathWithoutData = aFilePath.SubStr(5, aFilePath.Size());

		if (anOptionalNewExtension.Empty() == false)
		{
			int extensionIndex = pathWithoutData.RFind(".");
			pathWithoutData = pathWithoutData.SubStr(0, extensionIndex + 1);

			pathWithoutData += anOptionalNewExtension;
		}

		CU::String generatedDataFilePath = "GeneratedData/";
		generatedDataFilePath += pathWithoutData;

		return generatedDataFilePath;
	}

	//If OptionalExtension is blank, the outputstring will have the same extension as the input string
	//OptionalExtension needs to be entered without a period, "xml", NOT ".xml"
	inline CU::String GetRealDataFolderFilePath(const CU::String& aFilePath, const CU::String& anOptionalNewExtension = "")
	{
		CU::String pathWithoutData = aFilePath.SubStr(14, aFilePath.Size());

		if (anOptionalNewExtension.Empty() == false)
		{
			int extensionIndex = pathWithoutData.RFind(".");
			pathWithoutData = pathWithoutData.SubStr(0, extensionIndex + 1);

			pathWithoutData += anOptionalNewExtension;
		}

		CU::String generatedDataFilePath = "Data/";
		generatedDataFilePath += pathWithoutData;

		return generatedDataFilePath;
	}
	
	inline CU::String Concatenate(const char* aFormattedString, ...)
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

	inline void TrimWhiteSpacesAtBeginAndEnd(CU::String& aString)
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

	inline CU::String GetFileNameFromFilePath(const CU::String& aFilePath)
	{
		int index = aFilePath.RFind("/") + 1;

		CU::String result = aFilePath.SubStr(index, aFilePath.Size());
		return result;
	}

	inline CU::String ReplaceExtension(const CU::String& aFilePath, const CU::String& aNewExtension)
	{
		CU::String newString = aFilePath;
		int extensionIndex = newString.RFind(".");
		newString = newString.SubStr(0, extensionIndex);

		newString += aNewExtension;

		return newString;
	}
}