#pragma once

#define _WINSOCKAPI_

#include <memory>
#include <windows.h>
#include <vector>

#include "WindowSystem.h"
#include "DX11Renderer.h"
#include "ImGuiSystem.h"
#include "IWindow.h"

namespace common
{
	class WindowManager
	{
	public:
		WindowManager();
		~WindowManager() = default;

		void Initialize(UINT width, UINT hegiht, LPCWSTR name, UINT left = 0, UINT top = 0);
		bool BeginLoof();
		void EndLoof();
		void Finalize();

	private:
		std::unique_ptr<DX11Renderer> mRenderer;
		std::unique_ptr<WindowSystem> mWindowSystem;
	};

}
