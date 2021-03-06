#pragma once

#include <Ostaga.h>

#include <Layer.h>

namespace Ostaga {
	
	class Editor : public Layer
	{
	public:
		OSTAGA_IF_DEBUG(Editor() : Layer("Ostaga Editor") {})

		virtual void OnStartup() override;
		virtual void OnShutdown() override;

		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnRender() override;
		virtual void OnEvent(Event &e) override;

		virtual void OnGui() override;
	};
}