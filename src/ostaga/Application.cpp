
// Precompiled
#include "ospch.h"
//

#include <Ostaga.h>

#include "Application.h"

#include <TimeStep.h>
#include <TestingLayer.h>

#include <AudioDevice.h>
#include <Renderer.h>

#include <Editor.h>

namespace Ostaga {

	using namespace Audio;
	using namespace Graphics;

	Application *Application::s_Current = nullptr;

	Application::Application()
	{
		PROFILE_SESSION_BEGIN("Ostaga-Startup");
		PROFILE_FUNCTION();

		s_Current = this;

		WindowProps props = {
			1280,
			720,
			"Realms of Ostaga",
			OSTAGA_IF_DEBUG(WindowMode::Maximized, WindowMode::WindowedFullscreen),
			false,
			OSTAGA_IF_DEBUG(true, false)
		};

		m_Window = CreateScope<Window>(props);
		m_Window->SetEventCallback(std::bind(&::Ostaga::Application::OnEvent, this, std::placeholders::_1));
		m_Window->SetClearColor({ 0.f, 0.f, 0.f, 1.f });

		OSTAGA_IF_DEBUG(m_ImGui = CreateScope<ImGuiSurface>(*m_Window);)

		AudioDevice::Init();
		Renderer::Init({ 10000 });
		PROFILE_SESSION_END();
	}

	Application::~Application()
	{
		Renderer::Shutdown();
		AudioDevice::Shutdown();
	}

	void Application::Update(TimeStep ts)
	{
		OSTAGA_IF_DEBUG(m_ImGui->Begin();)
		m_Layers.BeginAll();
		m_Layers.UpdateAll(ts);

		// No need to be renderering if the window is
		// minimized.
		if (!m_Iconified)
		{
			OSTAGA_IF_DEBUG(dockspace->BeginCapture();)
			m_Layers.RenderAll();
			OSTAGA_IF_DEBUG(dockspace->EndCapture();)

			OSTAGA_IF_DEBUG(
				dockspace->ShowViewportWindow();
				m_Layers.OnGui();
			)
		}

		m_Layers.EndAll();
		OSTAGA_IF_DEBUG(m_ImGui->End();)
	}


	void Application::OnEvent(Event &e)
	{
		if (e.GetType() == EventType::WindowClose)
			m_Running = false;

		e.Dispatch<WindowIconified>([&](WindowIconified& e)
		{
			m_Iconified = e.iconified;
			return true;
		});

		e.Dispatch<MouseDown>([&](MouseDown &e) 
		{
			OSTAGA_IF_DEBUG(
				ViewportContentProps content = dockspace->GetViewportContentProps();
				e.x = (e.x - content.x) / content.width;
				e.y = (e.y - content.y) / content.height;
				,
				e.x /= m_Window->GetWidth();
				e.y /= m_Window->GetHeight();
			)

			return false;
		});

		m_Layers.OnEvent(e);
	}

	void Application::Run()
	{
		OSTAGA_IF_DEBUG(dockspace = CreateRef<ViewportDockspace>(m_Window->GetWidth(), m_Window->GetHeight());)
		OSTAGA_IF_DEBUG(PushOverlay(dockspace);)
		OSTAGA_IF_DEBUG(PushLayer(CreateRef<Editor>());)
		PushLayer(CreateRef<TestingLayer>());

		float lastTime = (float) glfwGetTime();

		m_Running = true;
		m_Window->SetVisible(true);

		while (m_Running)
		{
			float now = (float) glfwGetTime();
			TimeStep ts{now - lastTime};
			lastTime = now;

			OSTAGA_IF_DEBUG(
				m_Analytics.frameTime = (double) ts.GetDeltaTime();
				m_Analytics.framesPerSecond = 1.0 / m_Analytics.frameTime;
			)

			Update(ts);
			m_Window->Update();
		}
	}
}