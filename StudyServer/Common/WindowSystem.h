#pragma once

#include <windows.h>

namespace common
{
	class DX11Renderer;

	/// <summary>
	/// �����츦 �����ϴ� Ŭ���� 
	/// </summary>
	class WindowSystem
	{
	public:
		WindowSystem() = default;
		~WindowSystem() = default;

		/// <summary>
		/// �����츦 �ʱ�ȭ�մϴ�
		/// </summary>
		void Initalize(UINT width, UINT height, LPCWSTR name, UINT left = 0, UINT top = 0);

		/// <summary>
		/// ������ �޼����� ó���մϴ� 
		/// </summary>
		bool ProcessWindowMessage();

		/// <summary>
		/// �����츦 �����մϴ�
		/// </summary>
		void Finalize();

		/// <summary>
		/// ������ �ڵ��� ��ȯ�մϴ�
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