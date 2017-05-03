#pragma once
#include "CUString.h"
#include "CUMap.h"
#include <functional>
#include <map>
#include "GrowingArray.h"
#include <windows.h>
#include <thread>
#include <mutex>

namespace CU
{
	typedef std::function<void(const CU::String&)> callback_function_file;

	class FileWatcher
	{
	public:
		FileWatcher();
		~FileWatcher();

		/* Will check the file for includes and add them as well*/
		bool WatchFileChangeWithDependencies(const CU::String& aFile, callback_function_file aFunctionToCallOnChange);
		bool WatchFileChange(const CU::String& aFile, callback_function_file aFunctionToCallOnChange);


		void FlushChanges();
	private:
		void UpdateChanges(const CU::String& aDir);
		void OnFolderChange(const CU::String& aDir);
		bool WatchDirectory(const CU::String& aDir);
		void OnFileChange(const CU::String& aFile);
		CU::GrowingArray<WIN32_FIND_DATA> GetAllFilesInFolder(const CU::String& aDir);

		std::thread* myThread;

		typedef	Map<CU::String, CU::GrowingArray<WIN32_FIND_DATA>> FolderMap;
		FolderMap myFolders;

		CU::GrowingArray<CU::String> myFileChangedThreaded;
		CU::GrowingArray<CU::String> myFileChanged;
		Map<CU::String, CU::GrowingArray<callback_function_file> > myCallbacks;

		typedef Map<CU::String, CU::GrowingArray<CU::String>> Dependencies;
		Dependencies myDependencies;

		std::mutex myMutex;
		bool myShouldEndThread;
		bool myThreadIsDone;
	};

}