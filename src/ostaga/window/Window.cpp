
// Precompiled Header
#include "ospch.h"
//

#include "Window.h"

namespace Ostaga {

	bool Window::s_InitializedGLFW = false;

	Window::Window(const WindowProps &props)
	{
		PROFILE_FUNCTION();
		m_Data.props = props;
		InitWindow();
	}
	Window::~Window()
	{
		DestroyWindow();
	}
	void Window::Update()
	{
		PROFILE_FUNCTION();
		glfwPollEvents();
		glfwSwapBuffers(m_WindowPtr);

		OSTAGA_IF_DEBUG(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Window::SetWindowMode(WindowMode mode)
	{
		// TODO: Implement SetWindowMode functionality
	}
	
	void Window::InitWindow()
	{
		PROFILE_FUNCTION();
		if (!s_InitializedGLFW)
		{
			int status = glfwInit();
			ASSERT_CRITICAL(status, "Failed to initialize the GLFW library!");
			s_InitializedGLFW = true;
		}

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

		SetupEventCallback();

	}
	void Window::DestroyWindow()
	{
		glfwDestroyWindow(m_WindowPtr);
	}
	
	GLFWwindow *Window::CreateWindowHandle()
	{
		PROFILE_FUNCTION();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OSTAGA_OPENGL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OSTAGA_OPENGL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		switch (m_Data.props.mode)
		{
		case WindowMode::Fullscreen:
		{
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);

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
			m_Data.props.height = vidmode->height + 1; 
			// Bug? Video mode for the monitor changes unless height is +1 unit taller.

			glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

			glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
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
	void Window::SetupEventCallback()
	{
		PROFILE_FUNCTION();
		glfwSetWindowUserPointer(m_WindowPtr, (void*) &m_Data);

		glfwSetKeyCallback(m_WindowPtr, [](GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS:
				case GLFW_REPEAT:
				{
					KeyDown event{ key };

					if (data.EventCallback)
						data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyUp event{ key };

					if (data.EventCallback)
						data.EventCallback(event);
					break;
				}
			}
		});
		glfwSetMouseButtonCallback(m_WindowPtr, [](GLFWwindow *window, int button, int action, int mods)
		{
			WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseDown event{ button, xpos, ypos };

					if (data.EventCallback)
						data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseUp event{ button, xpos, ypos };

					if (data.EventCallback)
						data.EventCallback(event);
					break;
				}
			}
		});
		glfwSetCursorPosCallback(m_WindowPtr, [](GLFWwindow *window, double xpos, double ypos)
		{
			WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

			MouseMove event{ xpos, ypos };

			if (data.EventCallback)
				data.EventCallback(event);

		});
		glfwSetScrollCallback(m_WindowPtr, [](GLFWwindow *window, double xoffset, double yoffset)
		{
			WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
			
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			MouseScrolled event{ xoffset, yoffset, xpos, ypos };

			if (data.EventCallback)
				data.EventCallback(event);

		});
		glfwSetWindowCloseCallback(m_WindowPtr, [](GLFWwindow *window)
		{
			WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
			WindowClose event;

			if (data.EventCallback)
				data.EventCallback(event);
		});
		glfwSetWindowIconifyCallback(m_WindowPtr, [](GLFWwindow *window, int iconified)
		{
			WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
			WindowIconified event{ (bool) iconified };

			if (data.EventCallback)
				data.EventCallback(event);
		});
		

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
			{
				switch (severity)
				{
				case GL_DEBUG_SEVERITY_NOTIFICATION:	LOG_TRACE(message); break;
				case GL_DEBUG_SEVERITY_LOW:				LOG_WARN(message); break;
				case GL_DEBUG_SEVERITY_MEDIUM:			LOG_WARN(message); break;
				case GL_DEBUG_SEVERITY_HIGH:			LOG_CRITICAL(message); OSTAGA_DEBUG_BREAK(); break;
				}

			}, nullptr);
		)
	}
}