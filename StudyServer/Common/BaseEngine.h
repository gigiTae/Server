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
	/// ���� Ŭ���� 
	/// </summary>
	class BaseEngine
	{
	public:
		BaseEngine();
		~BaseEngine() = default;

		/// <summary>
		/// ������ �ʱ�ȭ�մϴ�
		/// </summary>
		void Initialize(UINT width, UINT hegiht, LPCWSTR name);

		/// <summary>
		/// ������ ���� ����
		/// </summary>
		void Process();

		/// <summary>
		/// ������ �����մϴ� 
		/// </summary>
		void Finalize();

	protected:
		virtual void StartProcess() abstract;
		virtual void Update() abstract;
		virtual void EndProcess() abstract;

		/// <summary>
		/// ���ο� �����츦 ����մϴ�
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
