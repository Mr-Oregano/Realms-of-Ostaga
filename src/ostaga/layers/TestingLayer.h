#pragma once

#include <Ostaga.h>
#include <Util.h>

#include <layers/Layer.h>
#include <assets/Shader.h>
#include <graphics/Renderer.h>

namespace Ostaga {

	using namespace Graphics;

	class TestingLayer : public Layer
	{
	public:
		float angle = 0.0f;
		
		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(TestingLayer() : Layer("TestingLayer") {})

		virtual void OnStart()
		{
			
		}

		virtual void OnStop()
		{
			
		}

		virtual void OnUpdate(TimeStep ts)
		{
			angle += 180 * ts; // 180 degrees per second
		}

		glm::vec4 RandColor()
		{
			return  {
				rand() / (float) RAND_MAX, 
				rand() / (float) RAND_MAX, 
				rand() / (float) RAND_MAX, 
				1.0f
			};
		}

		virtual void OnRender()
		{
			Renderer::BeginScene(glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f));
			
			static const int size = 64;

			srand(0);
			for (int i = -1; i < 1280 / size + 1; ++i)
				for (int j = -1; j < 720 / size + 1; ++j)
					Renderer::Draw({ size * (i + 0.5f),  size * (j + 0.5) }, glm::vec2{ size }, angle, RandColor());

			Renderer::EndScene();
		}

		virtual bool OnEvent(Event &e)
		{
			return false;
		}
	};
}
