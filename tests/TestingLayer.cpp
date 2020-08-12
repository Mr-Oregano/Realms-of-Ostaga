
// Precompiled
#include "ospch.h"
//

#include "TestingLayer.h"

#include <Input.h>
#include <KeyEvent.h>

#include <Renderer.h>

static const int TILE_SIZE = 64;

static const int WORLD_WIDTH = 30;
static const int WORLD_HEIGHT = 22;

namespace Ostaga {

	using namespace Graphics;
	using namespace Audio;

	void TestingLayer::OnStart()
	{
		LoadAssets();

		glm::vec2 scale = glm::vec2{ TILE_SIZE } / glm::vec2{ forest_tile.width * atlas->GetWidth(), forest_tile.height * atlas->GetHeight() };

		tiles.reserve(WORLD_WIDTH * WORLD_HEIGHT);
		for (int y = 0; y < WORLD_HEIGHT; ++y)
			for (int x = 0; x < WORLD_WIDTH; ++x)
				tiles.emplace_back(forest_tile, glm::vec2{ TILE_SIZE * x, TILE_SIZE * y }, glm::vec2{ TILE_SIZE });

		entities.reserve(Random::Integer() % 500);
		for (int i = 0; i < entities.capacity(); ++i)
		{
			const Graphics::TextureAtlasEntry &texture = RandomEntityTexture();

			glm::vec2 size = glm::vec2{ texture.width * atlas->GetWidth(), texture.height * atlas->GetHeight() } * scale;
			glm::vec2 pos{ Random::Float() * WORLD_WIDTH * TILE_SIZE, Random::Float() * WORLD_HEIGHT * TILE_SIZE - size.y / 2 };

			entities.emplace_back(texture, pos, size);
		}
		
		std::sort(entities.begin(), entities.end(), [](Entity &e1, Entity &e2) -> bool {
			return e1.pos.y	< e2.pos.y;
		});

		temptation_trk = IAudio::LoadFromFile("res/music/temptation.wav");
		musicPlayer = IAudioPlayer::Create(temptation_trk, { AudioMode::Loop });
		musicPlayer->SetGain(0.25f);
		musicPlayer->Play();
	}

	void TestingLayer::OnStop()
	{
	}

	void TestingLayer::OnUpdate(TimeStep ts)
	{
		PROFILE_FUNCTION();
		while (!monsters.empty() && monsters.front().soundPlayer->IsStopped())
			monsters.pop();
	}

	void TestingLayer::OnRender()
	{
		// TODO: Sort renderable entities. Entities with higher y-value 
		//		 must be drawn *before* entities with lower y-value
		//

		PROFILE_FUNCTION();
		Renderer::BeginScene(glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f));

		for (Tile &tile : tiles)
			Renderer::Draw(tile.pos, tile.size, tile.texture);

		for (Entity &entity : entities)
			Renderer::Draw(entity.pos, entity.size, entity.texture);

		Renderer::Draw({ 210, 15 }, { 400, 15 }, white, { 1, 0, 0, 1 });
		Renderer::Draw({ 210, 30 }, { 400, 15 }, white, { 0, 1, 0, 1 });
		Renderer::Draw({ 210, 45 }, { 400, 15 }, white, { 0, 0, 1, 1 });

		Renderer::EndScene();
	}

	void TestingLayer::OnEvent(Event &e)
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
			case GLFW_KEY_P: musicPlayer->Play(); return true;
			case GLFW_KEY_O: musicPlayer->Stop(); return true;
			case GLFW_KEY_I: musicPlayer->Pause(); return true;
			}

			return false;
		});

		e.Dispatch<MouseDown>([&](MouseDown &e) {
			if (e.button == GLFW_MOUSE_BUTTON_1)
			{
				float x = ((float)e.x / 1280.0f) - 0.5f;
				float z = ((float)e.y / 720.0f) - 0.5f;
				monsters.emplace(glm::vec3{ 10.0f * x, 1.0f, 10.0f * z });

				return true;
			}

			return false;

		});

	}

}