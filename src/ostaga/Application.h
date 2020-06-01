#pragma once

#include <window/Window.h>
#include <layers/Layer.h>

#include <memory>

namespace Ostaga
{
	class Application
	{
	public:
		Application();
		~Application();

		void Update(TimeStep ts);
		void OnEvent(Event &e);

		// TODO: consider implementing a State system
		inline void PushLayer(Layer *layer) { m_Layers.PushLayer(layer); }
		inline void PushOverlay(Layer *overlay) { m_Layers.PushOverlay(overlay); }

		inline Layer *PopLayer() { return m_Layers.PopLayer(); }
		inline Layer *PopOverlay() { return m_Layers.PopOverlay(); }
		//

		void Run();
		inline void Stop() { m_Running = false; }

	public:
		inline static Application& Get() { return *s_Current; }

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;

		LayerStack m_Layers;

	private:
		static Application *s_Current;
	};
}