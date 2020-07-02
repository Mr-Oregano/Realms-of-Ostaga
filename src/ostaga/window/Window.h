#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Ostaga.h>

#include <events/Event.h>
#include <events/KeyEvent.h>
#include <events/MouseEvent.h>
#include <events/SystemEvent.h>

namespace Ostaga {

	enum class WindowMode
	{
		Windowed,
		WindowedFullscreen,
		Fullscreen
	};
	struct WindowProps
	{
		int width;
		int height;

		const char *title;

		WindowMode mode;
		bool vysnc;
	};

	class Window
	{
	public:
		Window(const WindowProps& props);
		~Window();

		void Update();

		inline int GetWidth() const { return m_Data.props.width; }
		inline int GetHeight() const { return m_Data.props.height; }

		inline void SetEventCallback(std::function<void(Event&)> EventCallback) { m_Data.EventCallback = EventCallback; } 

		inline bool IsVysncEnabled() const { return m_Data.props.vysnc; }
		inline void SetVsync(bool vsync) { m_Data.props.vysnc = vsync; glfwSwapInterval(vsync ? 1 : 0); }

		void SetWindowMode(WindowMode mode);
		OSTAGA_IF_DEBUG(inline void SetClearColor(const glm::vec4 &color) { glClearColor(color.r, color.g, color.b, color.a); })

		inline bool IsVisible() const { return m_Visible; }
		inline void SetVisible(bool visible) {
			m_Visible = visible;

			if (m_Visible)
			{
				glfwShowWindow(m_WindowPtr);
				return;
			}

			glfwHideWindow(m_WindowPtr);
		}

	private:
		void InitWindow();
		GLFWwindow *CreateWindowHandle();
		void SetupEventCallback();
		void DestroyWindow();

	private:
		GLFWwindow *m_WindowPtr = nullptr;

		struct WindowData
		{
			WindowProps props;
			std::function<void(Event &)> EventCallback;

		} m_Data;

		bool m_Visible = false;

	private:
		static bool s_InitializedGLFW;

	};
}