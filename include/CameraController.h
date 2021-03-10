#pragma once

#include <Camera.h>

#include <Maths.h>
#include <TimeStep.h>
#include <Event.h>

#include <unordered_map>

namespace Ostaga {

	struct ControllerProps
	{
		float speed;
		float x;
		float y;
	};

	class CameraController
	{
	public:
		CameraController(
			const OrthoCamera &camera = OrthoCamera(-1, 1, 1, -1), 
			const ControllerProps &props = { 1.f, 0.f, 0.f });

		void OnEvent(Event &e);
		void OnUpdate(TimeStep ts);

		inline glm::vec3 &GetPosition() { return m_Position; }

		OrthoCamera &GetCamera();
		
	private:
		OrthoCamera m_Camera;

		glm::vec3 m_Position;
		std::unordered_map<int, bool> m_KeyMap;

		ControllerProps m_Props;
	};
}