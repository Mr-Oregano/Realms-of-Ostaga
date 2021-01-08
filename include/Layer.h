#pragma once

#include <Ostaga.h>
#include <Ref.h>

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
			Layer() = default;
		)
		//

		virtual ~Layer() = default;

		virtual void OnBegin() {}
		virtual void OnEnd() {}

		virtual void OnStartup() {}
		virtual void OnShutdown() {}

		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnRender() {}
		virtual void OnEvent(Event &e) {}

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

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> overlay);

		Ref<Layer> PopLayer();
		Ref<Layer> PopOverlay();

		void BeginAll();
		void EndAll();
		void UpdateAll(TimeStep ts);
		void RenderAll();

		void OnEvent(Event &e);
		void OnGui();

	private:
		std::vector<Ref<Layer>> m_Layers;
		std::vector<Ref<Layer>> m_Overlays;
	};
}