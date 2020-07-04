#pragma once

#include <util/Maths.h>
#include <util/Ref.h>

#include <assets/Texture.h>

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

		// Rotations are expected in radians.
		static void Draw(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color = glm::vec4{ 1 }, float rotation = 0); 
		static void Draw(const glm::vec2 &position, const glm::vec2 &size, const Ref<Assets::Texture> &texture, float rotation = 0);
		static void Draw(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const Ref<Assets::Texture> &texture, float rotation = 0);
		
		static void EndScene();
	
	private:
		static void Flush();

	};
} }