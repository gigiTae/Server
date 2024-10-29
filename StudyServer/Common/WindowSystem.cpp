#include "WindowSystem.h"


UINT common::WindowSystem::Width = 0;
UINT common::WindowSystem::Height = 0;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		common::WindowSystem::Width = (UINT)LOWORD(lParam); // Queue resize
		common::WindowSystem::Height = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void common::WindowSystem::Initalize(UINT width, UINT height, LPCWSTR name)
{
	mHInstance = ::GetModuleHandle(NULL);

	WNDCLASS wndClass{};
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = mHInstance;
	wndClass.hIcon = ::LoadIcon(mHInstance, IDI_APPLICATION);
	wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = name;

	RegisterClass(&wndClass);

	mHwnd = CreateWindow(name
		, name
		, WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX
		, 0, 0, width, height
		, NULL, NULL, mHInstance, NULL);

	::ShowWindow(mHwnd, SW_SHOWNORMAL);
	::UpdateWindow(mHwnd);

	// Initialize Direct3D
	if (!createDevice())
	{
		cleanupDeviceD3D();
		::UnregisterClassW(wndClass.lpszClassName, wndClass.hInstance);
		return;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(mHwnd);
	ImGui_ImplDX11_Init(mDevice, mDeviceContext);
}

bool common::WindowSystem::Process()
{
	MSG msg;
	while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			return false;
	}

	if (Width != 0 && Height != 0)
	{
		cleanupRenderTarget();
		mSwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
		Width = Height = 0;
		createRenderTarget();
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello");
	ImGui::End();

	ImGui::Render();
	mDeviceContext->OMSetRenderTargets(1, &mRTV, nullptr);
	constexpr FLOAT color[4] = {0.f,0.f,0.f,1.f};
	mDeviceContext->ClearRenderTargetView(mRTV,color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	mSwapChain->Present(0, 0);

	return true;
}

void common::WindowSystem::Finalize()
{
	::DestroyWindow(mHwnd);
	::UnregisterClassW(L"4Quset", mHInstance);
}

bool common::WindowSystem::createDevice()
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = mHwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr
		, D3D_DRIVER_TYPE_HARDWARE
		, nullptr
		, createDeviceFlags
		, featureLevelArray
		, 2
		, D3D11_SDK_VERSION
		, &sd, &mSwapChain
		, &mDevice
		, &featureLevel
		, &mDeviceContext);

	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags
			, featureLevelArray
			, 2, D3D11_SDK_VERSION
			, &sd, &mSwapChain
			, &mDevice
			, &featureLevel
			, &mDeviceContext);

	if (res != S_OK)
		return false;

	createRenderTarget();
	return true;
}

void common::WindowSystem::createRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	mSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	mDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRTV);
	pBackBuffer->Release();
}

void common::WindowSystem::cleanupDeviceD3D()
{
	cleanupRenderTarget();
	if (mSwapChain)
	{
		mSwapChain->Release(); mSwapChain = nullptr;
	}
	if (mDeviceContext)
	{
		mDeviceContext->Release(); mDeviceContext = nullptr;
	}
	if (mDevice)
	{
		mDevice->Release(); mDevice = nullptr;
	}
}

void common::WindowSystem::cleanupRenderTarget()
{
	if (mRTV)
	{
		mRTV->Release();
		mRTV = nullptr;
	}
}

