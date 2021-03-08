#pragma once

#include <Ostaga.h>
#include <Event.h>

#include <Maths.h>

namespace Ostaga {
	
	class OrthoCamera
	{
	public:
		OrthoCamera(float left, float right, float top, float bottom);

		void SetView(const glm::mat4 &view);
		void SetProj(const glm::mat4 &proj);

		glm::mat4 &GetViewProj();

	private:
		void CalcViewProj();

	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_ViewProjection;
	};
}