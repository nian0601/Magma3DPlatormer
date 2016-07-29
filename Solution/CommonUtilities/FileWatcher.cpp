#include "stdafx.h"

#include "FileWatcher.h"
#include <fstream>

namespace CU
{
	FileWatcher::FileWatcher()
		: myThread(nullptr)
		, myShouldEndThread(false)
		, myThreadIsDone(false)
		, myFileChanged(16)
		, myFileChangedThreaded(16)
	{

	}

	FileWatcher::~FileWatcher()
	{
		myShouldEndThread = true;
		while (!myThreadIsDone)
		{
			Sleep(1);
		}

		myThread->join();
		SAFE_DELETE(myThread);
	}

	void FileWatcher::FlushChanges()
	{
		std::lock_guard<std::mutex> guard(myMutex);


		CU::GrowingArray<CU::String<128>> temp(myFileChanged);
		myFileChanged = myFileChangedThreaded;
		myFileChangedThreaded = temp;
		
		for (CU::String<128>& theString : myFileChanged)
		{
			CU::String<128> directoryOfFile(theString);
			directoryOfFile = directoryOfFile.SubStr(0, directoryOfFile.RFind("\\/"));

			CU::String<128> theFile(theString);
			theFile = theFile.SubStr(theFile.RFind("\\/") + 1, theFile.Size());

			CU::GrowingArray<callback_function_file> callbacks = myCallbacks[theFile];
			for (int i = 0; i < callbacks.Size(); i++)
			{
				if (callbacks[i])
				{
					callbacks[i](theString);
				}
			}
		}

		myFileChanged.RemoveAll();
	}

	void FileWatcher::UpdateChanges(const CU::String<128>& aDir)
	{
		const DWORD timeOut = 1000;
		while (!myShouldEndThread)
		{
			HANDLE  ChangeHandle = FindFirstChangeNotification(aDir.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_ACTION_ADDED | FILE_ACTION_REMOVED | FILE_ACTION_MODIFIED);
			DWORD Wait = WaitForSingleObject(ChangeHandle, timeOut);
			if (Wait == WAIT_OBJECT_0)
			{
				myMutex.lock();
				OnFolderChange(aDir);
				FindNextChangeNotification(ChangeHandle);
				myMutex.unlock();
			}

			Sleep(1);
		}
		myThreadIsDone = true;
	}

	void FileWatcher::OnFolderChange(const CU::String<128>& aDir)
	{
		CU::GrowingArray<WIN32_FIND_DATA> currentFolderFiles = GetAllFilesInFolder(aDir);
		CU::GrowingArray<WIN32_FIND_DATA>& savedFolderFiles = myFolders[aDir];

		for (WIN32_FIND_DATA& currentFile : currentFolderFiles)
		{
			for (WIN32_FIND_DATA& savedFile : savedFolderFiles)
			{
				if (strcmp(currentFile.cFileName, savedFile.cFileName) == 0)
				{
					ULARGE_INTEGER currentFileTime;
					SYSTEMTIME currentFileTimeSystem;
					FileTimeToSystemTime(&currentFile.ftLastWriteTime, &currentFileTimeSystem);
					currentFileTime.LowPart = currentFile.ftLastWriteTime.dwLowDateTime;
					currentFileTime.HighPart = currentFile.ftLastWriteTime.dwHighDateTime;
					__int64 currentFileTime64 = currentFileTime.QuadPart;

					ULARGE_INTEGER savedFileTime;
					SYSTEMTIME savedFileTimeSystem;
					FileTimeToSystemTime(&savedFile.ftLastWriteTime, &savedFileTimeSystem);
					savedFileTime.LowPart = savedFile.ftLastWriteTime.dwLowDateTime;
					savedFileTime.HighPart = savedFile.ftLastWriteTime.dwHighDateTime;
					__int64 savedFileTime64 = savedFileTime.QuadPart;

					if (currentFileTime64 != savedFileTime64)
					{
						CU::String<128> fileThatChangedPath = aDir + "/" + CU::String<128>(currentFile.cFileName);
						bool isDependency = myDependencies.KeyExists(fileThatChangedPath);
						if (isDependency)
						{
							const CU::GrowingArray<CU::String<128>>& deps = myDependencies[fileThatChangedPath];
							for (const CU::String<128>& file : deps)
							{
								OnFileChange(file);
							}

						}
						else
						{
							OnFileChange(fileThatChangedPath);
						}

						// We have a change

						savedFile = currentFile;
					}
				}
			}
		}
	}

	void FileWatcher::OnFileChange(const CU::String<128>& aFile)
	{
		for (int i = 0; i < myFileChangedThreaded.Size(); i++)
		{
			if (myFileChangedThreaded[i] == aFile)
			{
				return;
			}
		}
		myFileChangedThreaded.Add(aFile);

	}

	bool FileWatcher::WatchFileChangeWithDependencies(const CU::String<128>& aFile, callback_function_file aFunctionToCallOnChange)
	{
		std::ifstream stream(aFile.c_str());
		if (!stream.good())
		{
			stream.close();
			return false;
		}

		CU::String<128> directoryOfFile(aFile);
		directoryOfFile = directoryOfFile.SubStr(0, directoryOfFile.RFind("\\/"));

		char fileLine[128];
		const CU::String<128> includeString = "include";
		while (stream.getline(fileLine, 128))
		{
			CU::String<128> line(fileLine);
			int found = line.Find(includeString);
			if (found != line.NotFound)
			{
				CU::String<128> foundFile(line);
				foundFile = foundFile.SubStr(foundFile.Find("\"") + 1, foundFile.Size());
				foundFile = foundFile.SubStr(0, foundFile.RFind("\""));

				if (!foundFile.Empty())
				{
					CU::String<128> depFile = directoryOfFile + "/" + foundFile;
					WatchFileChange(depFile, aFunctionToCallOnChange);
					myDependencies[depFile].Add(aFile);
				}
			}
		}

		stream.close();
		return WatchFileChange(aFile, aFunctionToCallOnChange);
	}

	bool FileWatcher::WatchFileChange(const CU::String<128>& aFile, callback_function_file aFunctionToCallOnChange)
	{
		std::ifstream stream(aFile.c_str());
		if (!stream.good())
		{
			stream.close();
			return false;
		}
		stream.close();

		CU::String<128> directoryOfFile(aFile);
		directoryOfFile = directoryOfFile.SubStr(0, directoryOfFile.RFind("\\/"));

		CU::String<128> theFile(aFile);
		theFile = theFile.SubStr(theFile.RFind("\\/") + 1, theFile.Size());

		myCallbacks[theFile].Add(aFunctionToCallOnChange);
		return WatchDirectory(directoryOfFile);
	}

	bool FileWatcher::WatchDirectory(const CU::String<128>& aDir)
	{
		if (myFolders.KeyExists(aDir) == true)
		{
			//Already in our watch list
			return true;
		}

		myFolders[aDir] = GetAllFilesInFolder(aDir);

		myThread = new std::thread(&FileWatcher::UpdateChanges, this, aDir);
		return true;
	}

	CU::GrowingArray<WIN32_FIND_DATA> FileWatcher::GetAllFilesInFolder(const CU::String<128>& aDir)
	{
		CU::GrowingArray<WIN32_FIND_DATA> filesInFolder(16);
		CU::String<128> searchDir(aDir);
		searchDir += "/*.*";
		WIN32_FIND_DATA fd;

		HANDLE hFind = ::FindFirstFile(searchDir.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					filesInFolder.Add(fd);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		return filesInFolder;
	}
}