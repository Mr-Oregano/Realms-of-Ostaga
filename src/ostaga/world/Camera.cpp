
// Precompiled
#include "ospch.h"
//

#include <Camera.h>

#include <KeyEvent.h>
#include <Input.h>

namespace Ostaga {

	OrthoCamera::OrthoCamera(float left, float right, float top, float bottom)
		: m_Projection(glm::ortho(left, right, bottom, top)), m_View({ 1.f })
	{
		CalcViewProj();
	}

	void OrthoCamera::SetView(const glm::mat4 &view)
	{
		m_View = view;
		CalcViewProj();
	}

	void OrthoCamera::SetProj(const glm::mat4 &proj)
	{
		m_Projection = proj;
		CalcViewProj();
	}

	glm::mat4 &OrthoCamera::GetViewProj()
	{
		return m_ViewProjection;
	}

	void OrthoCamera::CalcViewProj()
	{
		m_ViewProjection = m_Projection * glm::inverse(m_View);
	}

}