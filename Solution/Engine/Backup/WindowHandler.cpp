#include "stdafx.h"
#include "WindowHandler.h"


namespace Magma
{
	WindowHandler::WindowHandler(const CU::Vector2<float>& aWindowSize)
	{
		CU::String<30> title = "DirectX Window";

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WindowHandler::StaticWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hIcon = LoadIcon(wcex.hInstance, NULL);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = title.c_str();
		wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

		DL_ASSERT_EXP(RegisterClassEx(&wcex) != FALSE, "Failed to RegisterClassEx");

		RECT rc = { 0, 0, int(aWindowSize.x), int(aWindowSize.y) };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		myHwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			title.c_str(),
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			this);

		DL_ASSERT_EXP(myHwnd != nullptr, "Failed to CreateWindowEx");

		ShowWindow(myHwnd, 10);
		UpdateWindow(myHwnd);
	}


	WindowHandler::~WindowHandler()
	{
	}

	bool WindowHandler::PumpEvent()
	{
		
		if (PeekMessage(&myEventMsg, NULL, 0, 0, PM_REMOVE))
		{
			if (myEventMsg.message == WM_QUIT)
			{
				return false;
			}

			TranslateMessage(&myEventMsg);
			DispatchMessage(&myEventMsg);
		}

		return true;
	}

	LRESULT CALLBACK WindowHandler::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowHandler* directX = nullptr;
		if (message == WM_NCCREATE)
		{
			directX = static_cast<WindowHandler*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
			SetLastError(0);

			if (!SetWindowLongPtr(hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(directX)))
			{
				if (GetLastError() != 0)
				{
					DL_ASSERT("Failed to setup WndProc");
					return FALSE;
				}
			}
		}
		else
		{
			directX = reinterpret_cast<WindowHandler*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		if (directX != nullptr)
		{
			return directX->WndProc(hWnd, message, wParam, lParam);
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT CALLBACK WindowHandler::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATE:
			//CU::InputWrapper::GetInstance()->ToggleWindowActive();
			break;
		case WM_SIZE:
		{
			myNewSize.x = LOWORD(lParam);
			myNewSize.y = HIWORD(lParam);

			if (LOWORD(wParam) == SIZE_MAXIMIZED)
			{
				myShouldResize = true;
			}
			else if (LOWORD(wParam) == SIZE_RESTORED)
			{
				if (myIsResizing == false)
				{
					myShouldResize = true;
				}
			}
			break;
		}
		case WM_ENTERSIZEMOVE:
			myIsResizing = true;
			break;
		case WM_EXITSIZEMOVE:
			myIsResizing = false;
			myShouldResize = true;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

}