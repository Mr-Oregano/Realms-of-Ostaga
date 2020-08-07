#pragma once

#include <Ostaga.h>

#include <queue>
#include <vector>

#include <Layer.h>
#include <Entities.h>

#include <Renderer.h>
#include <IAudioPlayer.h>

#include <Ref.h>
#include <KeyEvent.h>

namespace Ostaga {

	using namespace Graphics;
	using namespace Audio;

	static const int TILE_SIZE = 64;

	static const int WORLD_WIDTH = 30;
	static const int WORLD_HEIGHT = 22;

	class TestingLayer : public Layer
	{
	public:
		std::queue<Monster> monsters;
		std::vector<Tile> tiles;

		Ref<IAudioPlayer> music;

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(TestingLayer() : Layer("TestingLayer") {})

		virtual void OnStart()
		{
			LoadAssets();

			tiles.reserve(WORLD_WIDTH * WORLD_HEIGHT);
			for (int y = 0; y < WORLD_HEIGHT; ++y)
			{
				for (int x = 0; x < WORLD_WIDTH; ++x)
				{
					tiles.emplace_back(forest_tile, glm::vec2{ TILE_SIZE * x, TILE_SIZE * y }, glm::vec2{ TILE_SIZE });
				}
			}

			music = IAudioPlayer::LoadFromFile("res/music/temptation.wav", { AudioMode::Loop });
			music->SetGain(0.25f);
			music->Play();
		}

		virtual void OnStop()
		{
		}

		virtual void OnUpdate(TimeStep ts)
		{
			PROFILE_FUNCTION();
			while (!monsters.empty() && monsters.front().sound->IsStopped())
				monsters.pop();
		}

		virtual void OnRender()
		{
			// TODO: Sort renderable entities. Entities with higher y-value 
			//		 must be drawn *before* entities with lower y-value
			//

			PROFILE_FUNCTION();
			Renderer::BeginScene(glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f));
		
			for (Tile &tile : tiles)
				tile.OnRender();

			// Night time
			Renderer::Draw({ 1280.0f / 2, 720.0f / 2 }, { 1280, 720 }, white, { 0.05f, 0.0f, 0.15f, 0.7f });
			//

			Renderer::Draw({ 210, 25 },  { 400, 30 }, white, { 1, 0, 0, 1 });
			Renderer::Draw({ 210, 65 },  { 400, 30 }, white, { 0, 1, 0, 1 });
			Renderer::Draw({ 210, 105 }, { 400, 30 }, white, { 0, 0, 1, 1 });

			Renderer::EndScene();
		}

		virtual void OnEvent(Event &e)
		{
			e.Dispatch<KeyUp>([&](KeyUp &e) {
				switch (e.keyCode)
				{
					case GLFW_KEY_Q:
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
					case GLFW_KEY_P: music->Play(); return true;
					case GLFW_KEY_O: music->Stop(); return true;
					case GLFW_KEY_I: music->Pause(); return true;
				}
				
				return false;
			});

			e.Dispatch<MouseDown>([&](MouseDown &e) {
				if (e.button == GLFW_MOUSE_BUTTON_1)
				{
					float x = ((float) e.x / 1280.0f) - 0.5f;
					float z = ((float) e.y / 720.0f) - 0.5f;
					monsters.emplace(glm::vec3{ 10.0f * x, 1.0f, 10.0f * z });

					return true;
				}

				return false;

			});

		}
	};
}
