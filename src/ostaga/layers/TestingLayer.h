#pragma once

#include <Ostaga.h>

#include <layers/Layer.h>
#include <graphics/Renderer.h>

#include <assets/Shader.h>
#include <assets/Texture.h>

#include <util/Ref.h>
#include <util/Random.h>

#include <events/KeyEvent.h>

namespace Ostaga {

	using namespace Graphics;
	using namespace Assets;

	class TestingLayer : public Layer
	{
	public:
		float angle = 0.0f;
		Ref<Texture> grass;
		Ref<Texture> tree;

		float xoffset = 0.0f;
		float yoffset = 0.0f;
		
		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(TestingLayer() : Layer("TestingLayer") {})

		virtual void OnStart()
		{
			grass = Texture::LoadFromFile("res/textures/grass.png");
			tree = Texture::LoadFromFile("res/textures/tree.png");
		}

		virtual void OnStop()
		{
		}

		virtual void OnUpdate(TimeStep ts)
		{
			static float angle = 0.0f;
			angle += glm::radians(50.0f * ts);
			xoffset = 256 * glm::cos(angle);
			yoffset = 256 * glm::sin(angle);
		}

		virtual void OnRender()
		{
			Renderer::BeginScene(glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f));
			
			static const int size = 64;
			static const int width = 2000;
			static const int height = 2000;
			static const int tileX = width / size;
			static const int tileY = height / size;
			static const int centerX = 1280 / 2;
			static const int centerY = 720 / 2;
			static const float spawnChance = 1 / 20.0f;

			for (float i = -tileX / 2; i < tileX / 2; ++i)
				for (float j = -tileY / 2; j < tileY / 2; ++j)
					Renderer::Draw({ size * i + centerX + xoffset,  size * j + centerY + yoffset }, glm::vec2{ size }, grass);

			Random::SetSeed(1);
			for (float j = -tileY / 2; j < tileY / 2; ++j)
				for (float i = -tileX / 2; i < tileX / 2; ++i)
					if (Random::Float() < spawnChance)
						Renderer::Draw({ size * i + centerX + xoffset,  size * j + centerY + yoffset }, glm::vec2{ 128 }, tree);

			Renderer::Draw({ 210, 25 }, { 400, 30 }, { 1, 0, 0, 1 });
			Renderer::Draw({ 210, 65 }, { 400, 30 }, { 0, 1, 0, 1 });
			Renderer::Draw({ 210, 105 }, { 400, 30 }, { 0, 0, 1, 1 });

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
