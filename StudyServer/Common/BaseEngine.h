#pragma once

#include <memory>
#include <windows.h>
#include <vector>

#include "WindowSystem.h"
#include "DX11Renderer.h"
#include "ImGuiSystem.h"
#include "IWindow.h"

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

		/// <summary>
		/// 엔진을 초기화합니다
		/// </summary>
		void Initialize(UINT width, UINT hegiht, LPCWSTR name);

		/// <summary>
		/// 엔진의 메인 루프
		/// </summary>
		void Process();

		/// <summary>
		/// 엔진을 종료합니다 
		/// </summary>
		void Finalize();

	protected:
		virtual void StartProcess() abstract;
		virtual void Update() abstract;
		virtual void EndProcess() abstract;

		/// <summary>
		/// 새로운 윈도우를 등록합니다
		/// </summary>
		template <typename T, typename ...Args>
		T* RegisterWindow(Args && ...args);

	private:
		std::unique_ptr<DX11Renderer> mRenderer;
		std::unique_ptr<WindowSystem> mWindowSystem;
		std::vector<std::unique_ptr<IWindow>> mWindows;
	};

	template <typename T, typename ...Args>
	T* common::BaseEngine::RegisterWindow(Args && ...args)
	{
		// r-value std::move(x)
		mWindows.push_back(std::make_unique<T>(std::forward<Args>(args)...));
		mWindows.back()->OnRegister();

		T* instance = static_cast<T*>(mWindows.back().get());
		return instance;
	}

}
