#include "stdafx.h"
#include "DL_Debug.h"
#include <cassert>
#include <cstdarg>
#include <ctime>
#include "DL_StackWalker.h"
#include <sys/types.h>
#include <sys/timeb.h>
#ifdef RELEASE_BUILD
#include <ShlObj.h>
#endif

DL_Debug::Debug* DL_Debug::Debug::ourInstance = nullptr;

DL_Debug::Debug::Debug()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[30];
	localtime_s(&tstruct, &now);

	strftime(buf, sizeof(buf), "%Y-%m-%d_%H_%M_%S", &tstruct);

	CU::String logFolder = "log\\";

	CreateDirectory("log", NULL);

	logFolder += buf;
	logFolder += "_";
	logFolder += "DebugLogger.txt";
	myDebugFile.open(logFolder.c_str());
}


DL_Debug::Debug::~Debug()
{
}

bool DL_Debug::Debug::Destroy()
{
	//if (ourInstance->myDebugFile.close() == false)
	//{
	//	return(false);
	//}

	ourInstance->myDebugFile.close();

	delete ourInstance;
	ourInstance = nullptr;
	return(true);
}

DL_Debug::Debug* DL_Debug::Debug::GetInstance()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new DL_Debug::Debug();
	}

	return ourInstance;
}

void DL_Debug::Debug::PrintMessage(const char* aString)
{
	ourInstance->myDebugFile << aString << "\n";
	ourInstance->myDebugFile.flush();
}

void DL_Debug::Debug::PrintMessageVA(const char *aFormattedString, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, aFormattedString);
	vsprintf_s(buffer, aFormattedString, args);
	perror(buffer);
	va_end(args);

	ourInstance->myDebugFile << buffer << "\n";
	ourInstance->myDebugFile.flush();
}

void DL_Debug::Debug::AssertMessageVA(const char *aFileName, int aLine, const char *aFunctionName, const CU::String& aFormattedString, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, aFormattedString);
	vsprintf_s(buffer, aFormattedString.c_str(), args);
	perror(buffer);
	va_end(args);

	AssertMessage(aFileName, aLine, aFunctionName, buffer);
}

void DL_Debug::Debug::AssertMessage(bool aAssertExpression, const char *aFileName, int aLine, const char *aFunctionName, const CU::String& aString)
{
	if (aAssertExpression == false)
	{
		AssertMessage(aFileName, aLine, aFunctionName, aString);
	}
}

void DL_Debug::Debug::AssertMessage(const char *aFileName, int aLine, const char *aFunctionName, const CU::String& aString)
{
	myTempString.Clear();
	myTempString += "\nError Message: ";
	myTempString += aString;
	myTempString += "\n\n";
	myTempString += "File: ";
	myTempString += aFileName;
	myTempString += "\n\n";
	myTempString += "Line: ";
	myTempString += aLine;
	myTempString += "\n";
	myTempString += "Function: ";
	myTempString += aFunctionName;
	myTempString += "\n\n";

	ourInstance->myDebugFile << myTempString.c_str();
	ourInstance->myDebugFile << "\n" << "\n" << "Callstack" << "\n";

	DL_Debug::StackWalker sw;
	sw.ShowCallstack();
	ourInstance->myDebugFile.flush();

	const size_t cSize = strlen(myTempString.c_str()) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t tempSize;
	mbstowcs_s(&tempSize, wc, cSize, myTempString.c_str(), cSize);

	//_wassert(wc, 0, aLine);

	_wassert(wc, _CRT_WIDE(__FILE__), __LINE__);
	delete[] wc;
}

void DL_Debug::Debug::AssertMessage(bool aAssertExpression, const char *aFileName, int aLine, const char *aFunctionName, const char* aString)
{
	if (aAssertExpression == false)
	{
		AssertMessage(aFileName, aLine, aFunctionName, aString);
	}
}

void DL_Debug::Debug::AssertMessage(const char *aFileName, int aLine, const char *aFunctionName, const char* aString)
{
	ourInstance->myDebugFile << "\nError Message: ";
	ourInstance->myDebugFile << aString;
	ourInstance->myDebugFile << "\n\n";
	ourInstance->myDebugFile << "File: ";
	ourInstance->myDebugFile << aFileName;
	ourInstance->myDebugFile << "\n\n";
	ourInstance->myDebugFile << "Line: ";
	ourInstance->myDebugFile << aLine;
	ourInstance->myDebugFile << "\n";
	ourInstance->myDebugFile << "Function: ";
	ourInstance->myDebugFile << aFunctionName;
	ourInstance->myDebugFile << "\n\n";

	ourInstance->myDebugFile << "\n" << "\n" << "Callstack" << "\n";

	DL_Debug::StackWalker sw;
	sw.ShowCallstack();
	ourInstance->myDebugFile.flush();

	const size_t cSize = strlen(aString) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t tempSize;
	mbstowcs_s(&tempSize, wc, cSize, aString, cSize);

	//_wassert(wc, 0, aLine);

	_wassert(wc, _CRT_WIDE(__FILE__), __LINE__);
	delete[] wc;
}

void DL_Debug::Debug::DebugMessage(const char *aFileName, int aLine, const char *aFunctionName, const char *aFormattedString, ...)
{
	ourInstance->myDebugFile << "\n" << "File: " << aFileName << "\n" << "Line: " << aLine << "\n" << "Function: " << aFunctionName << "\n";
	char buffer[1024];
	va_list args;
	va_start(args, aFormattedString);
	vsprintf_s(buffer, aFormattedString, args);
	perror(buffer);
	va_end(args);

	ourInstance->myDebugFile << buffer << "\n";
	ourInstance->myDebugFile.flush();
}

void DL_Debug::Debug::ShowMessageBox(HWND aHwnd, LPCSTR aText, LPCSTR aTitle, UINT aType)
{
	MessageBox(aHwnd, aText, aTitle, aType);
}