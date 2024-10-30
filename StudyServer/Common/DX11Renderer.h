#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace common
{
	/// <summary>
	/// DX11 ·£´õ·¯ 
	/// </summary>
	class DX11Renderer
	{
	public:
		DX11Renderer() = default;
		~DX11Renderer() = default;

		bool Initialize(HWND hwnd, UINT width, UINT height);
		void EndRender();
		void Finalize();
		void Resize(UINT width, UINT height);
		void BeginRender();
		ID3D11Device* GetDevice()const { return mDevice; }
		ID3D11DeviceContext* GetDeviceContext()const { return mDeviceContext; }

	private:
		void clearRenderTarget();
		bool createDevice();
		void cleanupRenderTarget();
		void cleanupDeviceD3D();
		void createRenderTarget();

	private:
		HWND mHwnd;
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		IDXGISwapChain* mSwapChain;
		ID3D11RenderTargetView* mRTV;
	};

}
