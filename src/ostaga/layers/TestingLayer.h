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
			Renderer::Init();
		}

		virtual void OnStop()
		{
			Renderer::Shutdown();
		}

		virtual void OnUpdate(TimeStep ts)
		{
			angle += 180 * ts; // 180 degrees per second
		}

		virtual void OnRender()
		{
			Renderer::BeginScene(glm::ortho(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f));
			Renderer::Draw({ 0.0f, 0.0f }, { 0.5f, 0.5f }, angle);
			Renderer::EndScene();
		}

		virtual bool OnEvent(Event &e)
		{
			return false;
		}
	};
}
