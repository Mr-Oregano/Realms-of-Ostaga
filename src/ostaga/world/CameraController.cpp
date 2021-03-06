
// Precompiled
#include "ospch.h"
//

#include <CameraController.h>

#include <Input.h>
#include <KeyEvent.h>

namespace Ostaga {

	CameraController::CameraController(const OrthoCamera &camera, const ControllerProps &props)
		: m_Camera(camera), m_Props(props)
	{
		m_Position = { props.x, props.y, 0.f };
	}

	void CameraController::OnEvent(Event &e)
	{
		e.Dispatch<KeyDown>([&](KeyDown &e)
		{
			m_KeyMap[e.keyCode] = true;
			return true;
		});

		e.Dispatch<KeyUp>([&](KeyUp &e)
		{
			m_KeyMap[e.keyCode] = false;
			return true;
		});
	}

	void CameraController::OnUpdate(TimeStep ts)
	{
		PROFILE_FUNCTION();
		glm::vec3 dir = { 0.f, 0.f, 0.f };

		dir.x = (float)(m_KeyMap[GLFW_KEY_D] - m_KeyMap[GLFW_KEY_A]);
		dir.y = (float)(m_KeyMap[GLFW_KEY_S] - m_KeyMap[GLFW_KEY_W]);

		if (dir != glm::vec3{ 0.f, 0.f, 0.f })
			dir = glm::normalize(dir);

		m_Position += dir * (m_Props.speed * ts);

		glm::mat4 view = glm::translate(glm::mat4{1.f}, m_Position);
		m_Camera.SetView(view);
	}

	OrthoCamera &CameraController::GetCamera()
	{
		return m_Camera;
	}
}