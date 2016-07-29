#pragma once

#ifdef RELEASE_BUILD
#define DL_ASSERT(string) DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__,__LINE__,__FUNCTION__, string)
#define DL_ASSERT_EXP(expression, string)
#define DL_PRINT(string) DL_Debug::Debug::GetInstance()->PrintMessage(string)
#define DL_PRINT_VA(...)
#define DL_DEBUG( ... ) DL_Debug::Debug::GetInstance()->DebugMessage(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define DL_MESSAGE_BOX(ERRORTEXT, TITLETEXT, TYPE)

#else

#define DL_ASSERT(string) DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__,__LINE__,__FUNCTION__, string)
#define DL_ASSERT_VA(...) DL_Debug::Debug::GetInstance()->AssertMessageVA(__FILE__,__LINE__,__FUNCTION__, __VA_ARGS__)
#define DL_ASSERT_EXP(expression, string) DL_Debug::Debug::GetInstance()->AssertMessage(expression, __FILE__,__LINE__,__FUNCTION__, string)
#define DL_PRINT(string)  DL_Debug::Debug::GetInstance()->PrintMessage(string)
#define DL_PRINT_VA( ... ) DL_Debug::Debug::GetInstance()->PrintMessageVA(__VA_ARGS__)
#define DL_DEBUG( ... )  DL_Debug::Debug::GetInstance()->DebugMessage(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define DL_MESSAGE_BOX(ERRORTEXT, TITLETEXT, TYPE) DL_Debug::Debug::GetInstance()->ShowMessageBox(NULL, ERRORTEXT, TITLETEXT, TYPE);
#endif


#include "CUString.h"
#include <fstream>
#include "DL_Assert.h"
#include "DL_StackWalker.h"

namespace DL_Debug
{
	class Debug
	{
	public:
		static bool Destroy();
		static Debug* GetInstance();

		void PrintMessage(const char* aString);
		void PrintMessageVA(const char *aFormattedString, ...);
		void DebugMessage(const char *aFileName, int aLine, const char *aFunctionName, const char *aFormattedString, ...);
		void AssertMessageVA(const char *aFileName, int aLine, const char *aFunctionName, const CU::String<256>& aFormattedString, ...);
		void AssertMessage(bool aAssertExpression, const char *aFileName, int aLine, const char *aFunctionName, const CU::String<256>& aString);
		void AssertMessage(const char *aFileName, int aLine, const char *aFunctionName, const CU::String<256>& aString);
		void ShowMessageBox(HWND aHwnd, LPCSTR aText, LPCSTR aTitle, UINT aType);
	private:
		Debug();
		~Debug();

		static Debug* ourInstance;
		std::ofstream myDebugFile;
	};
}