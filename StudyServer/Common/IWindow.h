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
		virtual void RenderGUI() abstract;
	};
}