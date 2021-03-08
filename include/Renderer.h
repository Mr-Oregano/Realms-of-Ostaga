#pragma once

#include <Maths.h>
#include <Ref.h>

#include <TextureAtlas.h>
#include <Shader.h>
#include <Geometry.h>

namespace Ostaga { namespace Graphics {

	struct RendererProps
	{
		size_t batchCapacity;
	};

	struct RendererAnalytics
	{
		int drawCalls;
		int submissions;
		Ref<TextureAtlas> currentAtlas;
	};

	class Renderer {

	public:
		static void Init(const RendererProps& props);
		static void Shutdown();

		static void BeginScene(const glm::mat4 &camera);
		static void SetTextureAtlas(const Ref<TextureAtlas>& atlas);

		static RendererAnalytics GetAnalytics();

		static void Draw(Geometry &geometry);

		// Rotations are expected in radians.
		static void Draw(
			const glm::vec2 &pos, 
			const glm::vec2 &size, 
			const TextureAtlasEntry &tex, 
			const glm::vec4 &tint = glm::vec4{1.0f}, 
			float rotation = 0);
		
		static void EndScene();
	
	private:
		static void Flush();

	};
} }