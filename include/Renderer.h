#pragma once

#include <Maths.h>
#include <Ref.h>

#include <TextureAtlas.h>
#include <Shader.h>
#include <Geometry.h>

#include <Scene.h>

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
		
		static void Draw(Scene &scene);

		static RendererAnalytics GetAnalytics();
			
	private:
		static void SetTextureAtlas(const Ref<TextureAtlas>& atlas);
		
		static void BeginScene(const glm::mat4 &camera);
		static void EndScene();

		static void Flush();

	};
} }