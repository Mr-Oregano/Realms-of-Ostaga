
// Precompiled
#include "ospch.h"
//

#include "Application.h"

#include <layers/TestingLayer.h>

namespace Ostaga {

	Application *Application::s_Current = nullptr;

	Application::Application()
	{
		s_Current = this;

		WindowProps props = {
			1280,
			720,
			"Realms of Ostaga",
			OSTAGA_DEBUG_WRAP(WindowMode::Windowed, WindowMode::WindowedFullscreen),
			false 
		};

		m_Window = std::make_unique<Window>(props);
		m_Window->SetEventCallback(std::bind(&::Ostaga::Application::OnEvent, this, std::placeholders::_1));

	}
	Application::~Application()
	{
	}

	void Application::Update()
	{
		m_Layers.OnUpdate();
		m_Layers.OnRender();
	}

	void Application::OnEvent(Event &e)
	{
		if (e.GetType() == EventType::WindowClose)
			m_Running = false;

		m_Layers.OnEvent(e);
	}

	void Application::Run()
	{
		PushLayer(new TestingLayer);

		m_Running = true;
		m_Window->SetVisible(true);

		while (m_Running)
		{
			Update();
			m_Window->Update();
		}
	}
}