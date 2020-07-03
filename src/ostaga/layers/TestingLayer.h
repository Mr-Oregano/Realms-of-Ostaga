#pragma once

#include <Ostaga.h>
#include <util/Ref.h>

#include <layers/Layer.h>
#include <assets/Shader.h>
#include <graphics/Renderer.h>

#include <util/Random.h>

#include <events/KeyEvent.h>

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
			static const float speed = glm::radians(180.0f);
			angle += speed * ts; // radians per second
		}

		glm::vec4 RandColor()
		{
			return  {
				Random::Float(), 
				Random::Float(),
				Random::Float(),
				1.0f
			};
		}

		virtual void OnRender()
		{
			Renderer::BeginScene(glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f));
			
			static const int size = 128;

			Random::SetSeed(1);
			for (float i = -0.5f; i < 1280 / size + 1.5f; ++i)
				for (float j = -0.5f; j < 720 / size + 1.5f; ++j)
					Renderer::Draw({ size * i,  size * j }, glm::vec2{ size }, angle, RandColor());

			Renderer::EndScene();
		}

		virtual void OnEvent(Event &e)
		{
			e.Dispatch<KeyDown>([](KeyDown &e) {
				if (e.keyCode == GLFW_KEY_W)
				{
					LOG_INFO("Foward");
					return true;
				}

				return false;
			});

		}
	};
}
