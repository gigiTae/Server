#include "BaseEngine.h"

#include "ImGuiSystem.h"

common::BaseEngine::BaseEngine()
	:mWindowSystem(std::make_unique<WindowSystem>())
	, mRenderer(std::make_unique<DX11Renderer>())
{}


void common::BaseEngine::InitializeEngine(UINT width, UINT hegiht, LPCWSTR name)
{
	mWindowSystem->Initalize(width, hegiht, name);
	HWND hwnd = mWindowSystem->GetHWND();

	mRenderer->Initialize(hwnd, width, hegiht);
	ID3D11Device* device = mRenderer->GetDevice();
	ID3D11DeviceContext* deviceContext = mRenderer->GetDeviceContext();
	ImGuiSystem::Initialize(hwnd, device, deviceContext);
}


void common::BaseEngine::Process()
{
	bool isDone = false;

	while (!isDone && mWindowSystem->ProcessWindowMessage())
	{
		if (WindowSystem::Width != 0 && WindowSystem::Height != 0)
		{
			mRenderer->Resize(WindowSystem::Width, WindowSystem::Height);
			WindowSystem::Width = WindowSystem::Height = 0;
		}
		ImGuiSystem::NewFrame();
		Update();
		mRenderer->BeginRender();
		RenderGUI();
		ImGuiSystem::RenderDrawData();
		mRenderer->EndRender();
	}
}

void common::BaseEngine::FinalizeEngine()
{
	ImGuiSystem::Finalize();
	mRenderer->Finalize();
	mWindowSystem->Finalize();
}

