#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Ostaga.h>

#include <Event.h>
#include <KeyEvent.h>
#include <MouseEvent.h>
#include <SystemEvent.h>

namespace Ostaga {

	enum class WindowMode
	{
		Windowed,
		WindowedFullscreen,
		Fullscreen,
		Maximized,
	};

	struct WindowProps
	{
		int width;
		int height;
		const char *title;
		WindowMode mode;
		bool vysnc;
		bool resizable;
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
		inline GLFWwindow* NativeWindowHandle() const { return m_WindowPtr; }

		inline bool IsVysncEnabled() const { return m_Data.props.vysnc; }
		inline void SetVsync(bool vsync) { m_Data.props.vysnc = vsync; glfwSwapInterval(vsync ? 1 : 0); }

		void SetWindowMode(WindowMode mode);
		inline void SetClearColor(const glm::vec4 &color) { glClearColor(color.r, color.g, color.b, color.a); }

		inline bool IsVisible() const { return m_Visible; }
		void SetVisible(bool visible);

	private:
		void InitWindow();
		void CreateWindowHandle();
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