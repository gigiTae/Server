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
		virtual void RenderGUI() abstract;
	};
}