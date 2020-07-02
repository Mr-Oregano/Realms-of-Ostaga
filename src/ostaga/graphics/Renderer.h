#pragma once

#include <Maths.h>

namespace Ostaga { namespace Graphics {

	namespace Renderer {

		void Init();
		void Shutdown();

		void BeginScene(const glm::mat4 &camera);
		void Draw(const glm::vec2 &position, const glm::vec2 &size, float rotation = 0.0f); // Rotations are expected in degrees.
		void EndScene();
	
	}
} }