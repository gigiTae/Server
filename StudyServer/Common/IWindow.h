#pragma once

#include <imgui.h>

namespace common
{
	/// <summary>
	/// ImGui â �������̽�
	/// </summary>
	class IWindow
	{
	public:
		virtual void Update() abstract;
		virtual void RenderGUI() abstract;
		virtual void OnRegister() abstract;
		virtual void OnDestroy() abstract;
	};
}