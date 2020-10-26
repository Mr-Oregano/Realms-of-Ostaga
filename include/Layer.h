#pragma once

#include <Ostaga.h>

#include <Event.h>
#include <KeyEvent.h>
#include <MouseEvent.h>
#include <SystemEvent.h>

#include <TimeStep.h>

namespace Ostaga {

	class Layer
	{
	public:
		// Debugging functionality
		OSTAGA_IF_DEBUG(
			Layer(const std::string &name = "Unnamed-Layer") : m_Name(name) {},
			Layer() = default;)
		//

		virtual ~Layer() = default;

		virtual void OnStart() = 0;
		virtual void OnStop() = 0;

		virtual void OnUpdate(TimeStep ts) = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(Event &e) = 0;

		virtual void OnGui() {}

	public:
		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(mutable std::string m_Name;)
		OSTAGA_IF_DEBUG(mutable bool m_Visible = true;)
	};

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);

		Layer* PopLayer();
		Layer* PopOverlay();

		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnEvent(Event &e);
		void OnGui();

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*> m_Overlays;
	};
}