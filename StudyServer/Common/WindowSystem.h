#pragma once

#include <windows.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

namespace common
{
	/// <summary>
	/// 윈도우를 관리하는 클래스 
	/// </summary>
	class WindowSystem
	{
	public:
		WindowSystem() = default;
		~WindowSystem() = default;

		void Initalize(UINT width, UINT height, LPCWSTR name);
		bool Process();
		void Finalize();

	private:
		bool createDevice();
		void createRenderTarget();
		void cleanupDeviceD3D();
		void cleanupRenderTarget();

	public:
		static UINT Width;
		static UINT Height;
	private:
		HWND mHwnd;
		HINSTANCE mHInstance;

		// d3d11
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		IDXGISwapChain* mSwapChain;
		ID3D11RenderTargetView* mRTV;
	};
}