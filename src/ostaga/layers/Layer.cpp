
// Precompiled
#include "ospch.h"
//

#include "Layer.h"

namespace Ostaga {

	LayerStack::~LayerStack()
	{
		for (auto& layer : m_Layers)
			layer->OnShutdown();
	}

	void LayerStack::PushLayer(Ref<Layer> layer)
	{
		m_Layers.push_back(layer);
		layer->OnStartup();
	}

	void LayerStack::PushOverlay(Ref<Layer> overlay)
	{
		m_Overlays.push_back(overlay);
		overlay->OnStartup();
	}

	Ref<Layer> LayerStack::PopLayer()
	{
		auto& last = *m_Layers.end();
		last->OnShutdown();
		m_Layers.erase(m_Layers.end());
		return last;
	}

	Ref<Layer> LayerStack::PopOverlay()
	{
		auto& last = *m_Overlays.end();
		last->OnShutdown();
		m_Overlays.erase(m_Overlays.end());
		return last;
	}

	void LayerStack::BeginAll()
	{
		for (auto& overlay : m_Overlays)
			overlay->OnBegin();

		for (auto& layer : m_Layers)
			layer->OnBegin();
	}

	void LayerStack::EndAll()
	{
		auto it = m_Layers.rbegin();
		for (; it != m_Layers.rend(); ++it)
			(*it)->OnEnd();

		it = m_Overlays.rbegin();
		for (; it != m_Overlays.rend(); ++it)
			(*it)->OnEnd();
	}

	void LayerStack::UpdateAll(TimeStep ts)
	{
		auto it = m_Overlays.rbegin();
		for (; it != m_Overlays.rend(); ++it)
			(*it)->OnUpdate(ts);

		it = m_Layers.rbegin();
		for (; it != m_Layers.rend(); ++it)
			(*it)->OnUpdate(ts);
	}

	void LayerStack::RenderAll()
	{
		for (auto& layer : m_Layers)
			OSTAGA_IF_DEBUG(if (layer->m_Visible))
				layer->OnRender();

		for (auto& overlay : m_Overlays)
			OSTAGA_IF_DEBUG(if (overlay->m_Visible))
				overlay->OnRender();
	}

	void LayerStack::OnEvent(Event &e)
	{
		auto it = m_Overlays.rbegin();
		for (; it != m_Overlays.rend(); ++it)
		{
			(*it)->OnEvent(e);

			// Event has been processed by the layer
			if (e.Handled)
				return;
		}

		it = m_Layers.rbegin();
		for (; it != m_Layers.rend(); ++it)
		{
			(*it)->OnEvent(e);

			// Event has been processed by the layer
			if (e.Handled)
				return;
		}
	}

	void LayerStack::OnGui()
	{
		auto it = m_Overlays.rbegin();
		for (; it != m_Overlays.rend(); ++it)
			(*it)->OnGui();

		it = m_Layers.rbegin();
		for (; it != m_Layers.rend(); ++it)
			(*it)->OnGui();
	}
}