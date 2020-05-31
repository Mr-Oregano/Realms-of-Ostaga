#pragma once

#include <window/Window.h>

#include <memory>

namespace Ostaga
{
	class Application
	{
	public:
		Application();
		~Application();

		void Update();
		void OnEvent(Event &e);

		void Run();
		inline void Stop() { m_Running = false; }

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;
	};
}