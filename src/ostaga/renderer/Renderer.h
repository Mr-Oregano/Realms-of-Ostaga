#pragma once

#include <util/Maths.h>
#include <util/Ref.h>

#include <assets/graphics/TextureAtlas.h>

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
		static void SetTextureAtlas(const Ref<Assets::TextureAtlas>& atlas);

		// Rotations are expected in radians.
		static void Draw(
			const glm::vec2 &pos, 
			const glm::vec2 &size, 
			const Assets::TextureAtlasEntry &tex, 
			const glm::vec4 &tint = glm::vec4{1.0f}, 
			float rotation = 0);
		
		static void EndScene();
	
	private:
		static void Flush();

	};
} }