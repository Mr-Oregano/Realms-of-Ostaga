#pragma once

#include<Debug.h>

#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include <Window.h>

namespace Ostaga {

	class ImGuiSurface
	{
	public:
		ImGuiSurface(const Window& window);
		~ImGuiSurface();

		void Begin();
		void End();

	private:
		ImVec2 m_DisplaySize;
		ImFont *m_Font = nullptr;
	};
}