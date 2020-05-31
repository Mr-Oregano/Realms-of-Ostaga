
// Precompiled
#include "ospch.h"
//

#include "Application.h"

namespace Ostaga {

	Application::Application()
	{

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
	}

	void Application::OnEvent(Event &e)
	{
		LOG_TRACE(e);
	}

	void Application::Run()
	{
		m_Running = true;
		m_Window->SetVisible(true);

		while (m_Running)
		{
			m_Window->Update();
		}
	}
}