#pragma once

#include <Debug.h>

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