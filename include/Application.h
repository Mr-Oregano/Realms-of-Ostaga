#pragma once

#include <Window.h>
#include <ImGuiSurface.h>
#include <ViewportDockspace.h>

#include <Layer.h>
#include <Ref.h>

namespace Ostaga
{
	struct ApplicationAnalytics
	{
		double frameTime;
		double framesPerSecond;
	};

	class Application
	{
	public:
		Application();
		~Application();

		void Update(TimeStep ts);
		void OnEvent(Event &e);

		inline ApplicationAnalytics GetAnalytics() { return m_Analytics; }

		// TODO: consider implementing a State system
		inline void PushLayer(Ref<Layer> layer) { m_Layers.PushLayer(layer); }
		inline void PushOverlay(Ref<Layer> overlay) { m_Layers.PushOverlay(overlay); }

		inline Ref<Layer> PopLayer() { return m_Layers.PopLayer(); }
		inline Ref<Layer> PopOverlay() { return m_Layers.PopOverlay(); }
		//

		void Run();
		inline void Stop() { m_Running = false; }

	public:
		inline static Application& Get() { return *s_Current; }

	private:
		Scope<Window> m_Window;
		OSTAGA_IF_DEBUG(Scope<ImGuiSurface> m_ImGui;)
		OSTAGA_IF_DEBUG(Ref<ViewportDockspace> dockspace;)
		ApplicationAnalytics m_Analytics;

		bool m_Running = false;
		bool m_Iconified = false;

		LayerStack m_Layers;

	private:
		static Application *s_Current;
	};
}