#pragma once

#include <Ostaga.h>

#include <layers/Layer.h>
#include <graphics/Renderer.h>

#include <assets/Shader.h>
#include <assets/TextureAtlas.h>

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

		TextureAtlasEntry grass;
		TextureAtlasEntry oaktree;
		TextureAtlasEntry pinetree;
		TextureAtlasEntry white;

		float xoffset = 0.0f;
		float yoffset = 0.0f;
		
		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(TestingLayer() : Layer("TestingLayer") {})

		virtual void OnStart()
		{
			atlas = TextureAtlas::Create("res/textures/atlas.png");

			grass =    atlas->AddEntry({ 1, 1, 16, 16 });
			oaktree =  atlas->AddEntry({ 32, 0, 64, 64 });
			pinetree = atlas->AddEntry({ 96, 0, 64, 64 });
			white =    atlas->AddEntry({ atlas->GetWidth() - 1, atlas->GetHeight() - 1, 1, 1 });
			
			Renderer::SetTextureAtlas(atlas);
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
			
			static const int TILE_SIZE = 64;
			
			static const int TILES_X = 30;
			static const int TILES_Y = 22;

			static const int centerX = 1280 / 2;
			static const int centerY = 720 / 2;
			
			static const float spawnChance = 1 / 5.0f;

			for (float i = -TILES_X / 2; i < TILES_X / 2; ++i)
				for (float j = -TILES_Y / 2; j < TILES_Y / 2; ++j)
					Renderer::Draw({ TILE_SIZE * i + centerX + xoffset,  TILE_SIZE * j + centerY + yoffset }, glm::vec2{ TILE_SIZE }, grass);

			Random::SetSeed(1);
			for (float j = -TILES_Y / 2; j < TILES_Y / 2; ++j)
			{
				for (float i = -TILES_X / 2; i < TILES_X / 2; ++i)
				{
					if (Random::Float() < spawnChance)
					{
						Renderer::Draw(
							{ TILE_SIZE * i + centerX + xoffset,  TILE_SIZE * j + centerY + yoffset },
							glm::vec2{ 128 },
							Random::Float() > 0.5f ? oaktree : pinetree);
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
