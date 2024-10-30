#include "ImGuiSystem.h"

#include <imgui_internal.h>

void common::ImGuiSystem::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);
}

void common::ImGuiSystem::Finalize()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void common::ImGuiSystem::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto imguiID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	ImGui::DockBuilderGetCentralNode(imguiID);
}

void common::ImGuiSystem::RenderDrawData()
{
	ImGui::Render(); 
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
