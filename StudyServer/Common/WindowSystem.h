#pragma once

#include <windows.h>

namespace common
{
	class DX11Renderer;

	/// <summary>
	/// 윈도우를 관리하는 클래스 
	/// </summary>
	class WindowSystem
	{
	public:
		WindowSystem() = default;
		~WindowSystem() = default;

		/// <summary>
		/// 윈도우를 초기화합니다
		/// </summary>
		void Initalize(UINT width, UINT height, LPCWSTR name, UINT left = 0, UINT top = 0);

		/// <summary>
		/// 윈도우 메세지를 처리합니다 
		/// </summary>
		bool ProcessWindowMessage();

		/// <summary>
		/// 윈도우를 종료합니다
		/// </summary>
		void Finalize();

		/// <summary>
		/// 윈도우 핸들을 반환합니다
		/// </summary>
		HWND GetHWND()const { return mHwnd; }

	public:
		static UINT Width;
		static UINT Height;
	private:
		HWND mHwnd;
		HINSTANCE mHInstance;
	};
}