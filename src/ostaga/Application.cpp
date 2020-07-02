
// Precompiled
#include "ospch.h"
//

#include "Application.h"

#include <TimeStep.h>

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
			OSTAGA_IF_DEBUG(WindowMode::Windowed, WindowMode::WindowedFullscreen),
			false 
		};

		m_Window = std::make_unique<Window>(props);
		m_Window->SetEventCallback(std::bind(&::Ostaga::Application::OnEvent, this, std::placeholders::_1));

	}
	Application::~Application()
	{
	}

	void Application::Update(TimeStep ts)
	{
		m_Layers.OnUpdate(ts);
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

		float lastTime = (float) glfwGetTime();

		m_Running = true;
		m_Window->SetVisible(true);

		while (m_Running)
		{
			float now = (float) glfwGetTime();
			TimeStep ts{now - lastTime};
			lastTime = now;

			Update(ts);
			m_Window->Update();
		}
	}
}