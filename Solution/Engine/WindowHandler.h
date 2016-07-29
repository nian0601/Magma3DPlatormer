#pragma once
#include <Vector.h>

namespace Magma
{
	class WindowHandler
	{
	public:
		WindowHandler(const CU::Vector2<float>& aWindowSize);
		~WindowHandler();

		bool PumpEvent();

		void HasResized();
		bool GetShouldResize() const;

		HWND GetHwnd() const;
		const CU::Vector2<float>& GetNewSize() const;

	private:

		static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HWND myHwnd;
		MSG myEventMsg;
		bool myIsResizing;
		bool myShouldResize;
		CU::Vector2<float> myNewSize;
	};

	inline void WindowHandler::HasResized()
	{
		myShouldResize = false;
	}

	inline bool WindowHandler::GetShouldResize() const
	{
		return myShouldResize;
	}

	inline HWND WindowHandler::GetHwnd() const
	{
		return myHwnd;
	}

	inline const CU::Vector2<float>& WindowHandler::GetNewSize() const
	{
		return myNewSize;
	}
}