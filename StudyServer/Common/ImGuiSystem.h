#pragma once

#include <windows.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <d3d11.h>

namespace common
{
	/// <summary>
	/// ImGui 도우미 함수
	/// </summary>
	class ImGuiSystem
	{
	public:
		static void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		static void Finalize();
		static void NewFrame();
		static void RenderDrawData();
	};
}

