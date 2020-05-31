
// Precompiled
#include "ospch.h"
//

#include "Layer.h"

namespace Ostaga {

	LayerStack::~LayerStack()
	{
		for (Layer *layer : m_Layers)
		{
			layer->OnStop();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer *layer)
	{
		m_Layers.push_back(layer);
		layer->OnStart();
	}
	void LayerStack::PushOverlay(Layer *overlay)
	{
		m_Overlays.push_back(overlay);
		overlay->OnStart();
	}

	Layer *LayerStack::PopLayer()
	{
		Layer *last = *m_Layers.end();
		last->OnStop();
		m_Layers.erase(m_Layers.end());
		return last;
	}
	Layer *LayerStack::PopOverlay()
	{
		Layer *last = *m_Overlays.end();
		last->OnStop();
		m_Overlays.erase(m_Overlays.end());
		return last;
	}

	void LayerStack::OnUpdate()
	{
		auto it = m_Overlays.rbegin();
		for (; it != m_Overlays.rend(); ++it)
			(*it)->OnUpdate();

		it = m_Layers.rbegin();
		for (; it != m_Layers.rend(); ++it)
			(*it)->OnUpdate();
	}
	void LayerStack::OnRender()
	{
		for (Layer *layer : m_Layers)
			OSTAGA_DEBUG_WRAP(if (layer->m_Visible))
				layer->OnRender();

		for (Layer *overlay : m_Overlays)
			OSTAGA_DEBUG_WRAP(if (overlay->m_Visible))
				overlay->OnRender();
	}

	void LayerStack::OnEvent(Event &e)
	{
		auto it = m_Overlays.rbegin();
		for (; it != m_Overlays.rend(); ++it)
		{
			// Event has been processed by the layer
			if ((*it)->OnEvent(e))
				return;
		}

		it = m_Layers.rbegin();
		for (; it != m_Layers.rend(); ++it)
		{
			// Event has been processed by the layer
			if ((*it)->OnEvent(e))
				return;
		}
	}
}