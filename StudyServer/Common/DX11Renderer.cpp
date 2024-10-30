#include "DX11Renderer.h"

bool common::DX11Renderer::Initialize(HWND hwnd, UINT width, UINT height)
{
	mHwnd = hwnd;

	if (!createDevice())
	{
		cleanupDeviceD3D();
		return false;
	}

	return true;
}

void common::DX11Renderer::EndRender()
{
	mSwapChain->Present(0, 0);
}

void common::DX11Renderer::Finalize()
{
	cleanupDeviceD3D();
}

bool common::DX11Renderer::createDevice()
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

void common::DX11Renderer::createRenderTarget()
{
	ID3D11Texture2D* pBackBuffer = nullptr;
	mSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	mDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRTV);
	pBackBuffer->Release();
}

void common::DX11Renderer::cleanupDeviceD3D()
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

void common::DX11Renderer::cleanupRenderTarget()
{
	if (mRTV)
	{
		mRTV->Release();
		mRTV = nullptr;
	}
}

void common::DX11Renderer::clearRenderTarget()
{
	mDeviceContext->OMSetRenderTargets(1, &mRTV, nullptr);
	constexpr FLOAT color[4] = { 0.f,0.f,0.f,1.f };
	mDeviceContext->ClearRenderTargetView(mRTV, color);
}

void common::DX11Renderer::BeginRender()
{
	clearRenderTarget();
}

void common::DX11Renderer::Resize(UINT width, UINT height)
{
	cleanupRenderTarget();
	mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	createRenderTarget();
}
