#include "WindowManager.h"

#include "ImGuiSystem.h"

common::WindowManager::WindowManager()
	:mWindowSystem(std::make_unique<WindowSystem>())
	, mRenderer(std::make_unique<DX11Renderer>())
{}

void common::WindowManager::Initialize(UINT width, UINT hegiht, LPCWSTR name, UINT left, UINT top)
{
	mWindowSystem->Initalize(width, hegiht, name, left, top);
	HWND hwnd = mWindowSystem->GetHWND();
	mRenderer->Initialize(hwnd, width, hegiht);
	ID3D11Device* device = mRenderer->GetDevice();
	ID3D11DeviceContext* deviceContext = mRenderer->GetDeviceContext();
	ImGuiSystem::Initialize(hwnd, device, deviceContext);
}

bool common::WindowManager::BeginLoof()
{
	// WindowMessage
	if (!mWindowSystem->ProcessWindowMessage())
	{
		return false;
	}

	// Resize
	if (WindowSystem::Width != 0 && WindowSystem::Height != 0)
	{
		mRenderer->Resize(WindowSystem::Width, WindowSystem::Height);
		WindowSystem::Width = WindowSystem::Height = 0;
	}

	ImGuiSystem::NewFrame();
	mRenderer->BeginRender();
}

void common::WindowManager::EndLoof()
{
	ImGuiSystem::RenderDrawData();
	mRenderer->EndRender();
}

void common::WindowManager::Finalize()
{
	ImGuiSystem::Finalize();
	mRenderer->Finalize();
	mWindowSystem->Finalize();
}


