#pragma once

#include <Maths.h>

namespace Ostaga { namespace Graphics {

	struct RendererProps
	{
		size_t batchCapacity;
	};

	class Renderer {

	public:
		static void Init(const RendererProps& props);
		static void Shutdown();

		static void BeginScene(const glm::mat4 &camera);

		// Rotations are expected in degrees.
		static void Draw(const glm::vec2 &position, const glm::vec2 &size, float rotation = 0.0f, const glm::vec4 &color = glm::vec4{ 1 }); 
		
		static void EndScene();
	
	private:
		static void Flush();

	};
} }