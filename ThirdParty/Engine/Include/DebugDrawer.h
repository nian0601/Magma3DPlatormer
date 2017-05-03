#pragma once

#include <CUString.h>


#define DEBUG_TEXT_X 10.f
#define DEBUG_TEXT_Y 10.f
#define DEBUG_TEXT_SPACING 10.f


#define DEBUG_TEXT(aString) Magma::DebugDrawer::GetInstance()->AddDebugText(aString);
#define DEBUG_PRINT(aValue) Magma::DebugDrawer::GetInstance()->AddDebugText(#aValue, aValue);


namespace Magma
{
	struct DebugText
	{
		DebugText(){}
		DebugText(const CU::String& aString, float aY)
			: myString(aString)
			, myY(aY)
		{}

		CU::String myString;
		float myY;
	};

	class DebugDrawer
	{
	public:
		static DebugDrawer* GetInstance();
		static void Destroy();

		void AddDebugText(const CU::String& aString);
		void AddDebugText(const CU::String& aString, float aValue);
		void AddDebugText(const CU::String& aString, int aValue);
		void AddDebugText(const CU::String& aString, const CU::String& aValue);
		void AddDebugText(const CU::String& aString, const CU::Vector2<float>& aValue);
		void AddDebugText(const CU::String& aString, const CU::Vector3<float>& aValue);
		void AddDebugText(const CU::String& aString, const CU::Vector4<float>& aValue);
		void AddDebugText(const CU::String& aString, const CU::Vector2<int>& aValue);
		void AddDebugText(const CU::String& aString, const CU::Vector3<int>& aValue);
		void AddDebugText(const CU::String& aString, const CU::Vector4<int>& aValue);



		const CU::GrowingArray<DebugText>& GetDebugTexts() const { return myDebugTexts; }
		void ClearDebugTexts() { myDebugTexts.RemoveAll(); }

	private:
		DebugDrawer();
		~DebugDrawer();

		float GetNextY() const;
		CU::GrowingArray<DebugText> myDebugTexts;

		static DebugDrawer* ourInstance;
	};
}