#pragma once

#include <Ostaga.h>

#include <layers/Layer.h>
#include <renderer/Renderer.h>

#include <assets/graphics/Shader.h>
#include <assets/graphics/TextureAtlas.h>

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
		Ref<TextureAtlas> atlas;

		TextureAtlasEntry forest_tile{ 0 };
		TextureAtlasEntry grass1{ 0 };
		TextureAtlasEntry grass2{ 0 };
		TextureAtlasEntry grass3{ 0 };
		TextureAtlasEntry grass4{ 0 };
		TextureAtlasEntry boulder{ 0 };
		TextureAtlasEntry oaktree{ 0 };
		TextureAtlasEntry pinetree{ 0 };
		TextureAtlasEntry white{ 0 };

		float xoffset = 0.0f;
		float yoffset = 0.0f;
		
		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(TestingLayer() : Layer("TestingLayer") {})

		virtual void OnStart()
		{
			atlas = TextureAtlas::Create("res/textures/atlas.png");

			forest_tile =	atlas->AddEntry({ 1, 1, 16, 16 });
			oaktree =		atlas->AddEntry({ 32, 0, 64, 64 });
			pinetree =		atlas->AddEntry({ 96, 0, 64, 64 });
			grass1 =		atlas->AddEntry({ 160, 0, 16, 16 });
			grass2 =		atlas->AddEntry({ 160, 16, 16, 16 });
			grass3 =		atlas->AddEntry({ 176, 0, 16, 16 });
			grass4 =		atlas->AddEntry({ 176, 16, 16, 16 });
			boulder =		atlas->AddEntry({ 192, 0, 64, 64 });
			white =			atlas->AddEntry({ atlas->GetWidth() - 1, atlas->GetHeight() - 1, 1, 1 });
			
			Renderer::SetTextureAtlas(atlas);
		}

		virtual void OnStop()
		{
		}

		virtual void OnUpdate(TimeStep ts)
		{
			PROFILE_FUNCTION();
			static float angle = 0.0f;
			angle += glm::radians(25.0f * ts);
			xoffset = 256 * glm::cos(angle);
			yoffset = 256 * glm::sin(angle);
		}

		std::pair<TextureAtlasEntry&, float> SpawnEntity()
		{
			float random = Random::Float();

			if (Random::Float() > 0.4f)
			{
				if (Random::Float() > 0.75f)
					return { grass1, 32.0f };
				else if (Random::Float() > 0.50f)
					return { grass2, 32.0f };
				else if (Random::Float() > 0.25f)
					return { grass3, 32.0f };
				else
					return { grass4, 32.0f };
			}

			if (Random::Float() > 0.50f)
				return { oaktree, 128.0f };
			else if (Random::Float() > 0.10f)
				return { pinetree, 128.0f };
			else
				return { boulder, 128.0f };
		}

		virtual void OnRender()
		{
			PROFILE_FUNCTION();
			Renderer::BeginScene(glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f));
			
			static const int TILE_SIZE = 64;
			
			static const int TILES_X = 30;
			static const int TILES_Y = 22;

			static const int centerX = 1280 / 2;
			static const int centerY = 720 / 2;
			
			static const float spawnChance = 1.0f;

			for (float i = -TILES_X / 2; i < TILES_X / 2; ++i)
				for (float j = -TILES_Y / 2; j < TILES_Y / 2; ++j)
					Renderer::Draw({ TILE_SIZE * i + centerX + xoffset,  TILE_SIZE * j + centerY + yoffset }, glm::vec2{ TILE_SIZE }, forest_tile);

			Random::SetSeed(1);
			for (float j = -TILES_Y / 2; j < TILES_Y / 2; ++j)
			{
				for (float i = -TILES_X / 2; i < TILES_X / 2; ++i)
				{
					if (Random::Float() < spawnChance)
					{
						auto [entity, size] = SpawnEntity();
						Renderer::Draw(
							{ TILE_SIZE * i + centerX + xoffset,
							  TILE_SIZE * j + centerY + yoffset - size / 2 },
							glm::vec2{ size },
							entity);
					}
				}
			}

			Renderer::Draw({ 210, 25 },  { 400, 30 }, white, { 1, 0, 0, 1 });
			Renderer::Draw({ 210, 65 },  { 400, 30 }, white, { 0, 1, 0, 1 });
			Renderer::Draw({ 210, 105 }, { 400, 30 }, white, { 0, 0, 1, 1 });

			Renderer::EndScene();
		}

		virtual void OnEvent(Event &e)
		{
			e.Dispatch<KeyUp>([](KeyUp &e) {
				if (e.keyCode == GLFW_KEY_Q)
				{
					static bool start = false;
					start = !start;

					if (start)
					{
						PROFILE_SESSION_BEGIN("Ostaga-Runtime");
						LOG_INFO("Start profiling session: Ostaga-Runtime");
					}
					else
					{
						PROFILE_SESSION_END();
						LOG_INFO("Ended profiling session: Ostaga-Runtime");
					}
					
					return true;
				}

				return false;
			});
		}
	};
}
