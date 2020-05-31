
// Precompiled Header
#include "ospch.h"
//

#include "Window.h"

namespace Ostaga {

	bool Window::s_InitializedGLFW = false;

	Window::Window(const WindowProps &props)
	{
		m_Data.props = props;
		InitWindow();
	}
	Window::~Window()
	{
		DestroyWindow();
	}
	void Window::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_WindowPtr);

		OSTAGA_DEBUG_WRAP(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Window::SetWindowMode(WindowMode mode)
	{
		// TODO
	}

	void Window::InitWindow()
	{
		if (!s_InitializedGLFW)
		{
			int status = glfwInit();
			ASSERT_CRITICAL(status, "Failed to initialize the GLFW library!");
			s_InitializedGLFW = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OSTAGA_OPENGL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OSTAGA_OPENGL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		m_WindowPtr = CreateWindowHandle();
		ASSERT_CRITICAL(m_WindowPtr, "Failed to create the window");

		glfwMakeContextCurrent(m_WindowPtr);
		SetVsync(m_Data.props.vysnc);

		if (!gladLoadGL())
		{
			LOG_CRITICAL("Failed to load required OpenGL functions");
			glfwTerminate();
			return;
		}

		glViewport(0, 0, m_Data.props.width, m_Data.props.height);

		LOG_INFO("OpenGL vendor: {0}", glGetString(GL_VENDOR));
		LOG_INFO("OpenGL renderer: {0}", glGetString(GL_RENDERER));
		LOG_INFO("OpenGL version: {0}", glGetString(GL_VERSION));
		LOG_INFO("GLSL version: {0}\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	GLFWwindow *Window::CreateWindowHandle()
	{
		switch (m_Data.props.mode)
		{
		case WindowMode::Fullscreen:
		{

			// In Fullscreen mode, width and height variables are ignored and instead
			// Re-assigned to the width and height of the the primary mointor's 
			// video mode.

			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);

			m_Data.props.width = vidmode->width;
			m_Data.props.height = vidmode->height;

			glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

			return glfwCreateWindow(
				m_Data.props.width, 
				m_Data.props.height,
				m_Data.props.title,
				monitor,
				nullptr);
		}

		case WindowMode::WindowedFullscreen:
		{
			// In WindowedFullscreen mode, width and height variables are ignored and instead
			// Re-assigned to the width and height of the the primary mointor's 
			// video mode.

			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);

			m_Data.props.width = vidmode->width;
			m_Data.props.height = vidmode->height;

			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

			return glfwCreateWindow(
				m_Data.props.width,
				m_Data.props.height,
				m_Data.props.title,
				nullptr,
				nullptr);
		}

		default:
		{
			return glfwCreateWindow(
				m_Data.props.width,
				m_Data.props.height,
				m_Data.props.title,
				nullptr,
				nullptr);
		}
		}
	}
	void Window::DestroyWindow()
	{
		glfwDestroyWindow(m_WindowPtr);
	}
}