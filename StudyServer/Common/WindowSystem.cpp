#include "WindowSystem.h"

#include "ImGuiSystem.h"

UINT common::WindowSystem::Width = 0;
UINT common::WindowSystem::Height = 0;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
				return 0;
			common::WindowSystem::Width = (UINT)LOWORD(lParam); // Queue resize
			common::WindowSystem::Height = (UINT)HIWORD(lParam);
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void common::WindowSystem::Initalize(UINT width, UINT height, LPCWSTR name, UINT left, UINT top)
{
	mHInstance = ::GetModuleHandle(NULL);

	WNDCLASS wndClass{};
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = mHInstance;
	wndClass.hIcon = ::LoadIcon(mHInstance, IDI_APPLICATION);
	wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = name;

	RegisterClass(&wndClass);

	mHwnd = CreateWindow(name
		, name
		, WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX
		, left, top, width, height
		, NULL, NULL, mHInstance, NULL);

	::ShowWindow(mHwnd, SW_SHOWNORMAL);
	::UpdateWindow(mHwnd);
}

bool common::WindowSystem::ProcessWindowMessage()
{
	MSG msg;
	while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			return false;
	}

	return true;
}



void common::WindowSystem::Finalize()
{
	::DestroyWindow(mHwnd);
	::UnregisterClassW(L"4Quset", mHInstance);
}

