#pragma once

#include <imgui.h>

namespace common
{
	/// <summary>
	/// ImGui 창 인터페이스
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