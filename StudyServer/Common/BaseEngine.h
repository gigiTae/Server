#pragma once

#include <memory>
#include <windows.h>

#include "WindowSystem.h"
#include "DX11Renderer.h"
#include "ImGuiSystem.h"

namespace common
{
	/// <summary>
	/// 엔진 클래스 
	/// </summary>
	class BaseEngine
	{
	public:
		BaseEngine();
		~BaseEngine() = default;

		void InitializeEngine(UINT width, UINT hegiht, LPCWSTR name);
		void Process();
		void FinalizeEngine();

	protected:
		virtual void Initialize() abstract;
		virtual void Finalize() abstract;
		virtual void Update() abstract;
		virtual void RenderGUI() abstract;

	private:
		std::unique_ptr<DX11Renderer> mRenderer;
		std::unique_ptr<WindowSystem> mWindowSystem;
	};

}
