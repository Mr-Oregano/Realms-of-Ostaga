#pragma once

#include <AudioDevice.h>
#include <IAudioPlayer.h>

#include <TextureAtlas.h>
#include <Renderer.h>

#include <Maths.h>
#include <Random.h>

namespace Ostaga {
	
	// TODO: Look into using an ECS

	extern Ref<Audio::IAudio> monster_soundfx;

	extern Ref<Graphics::TextureAtlas> atlas;
	extern Graphics::TextureAtlasEntry forest_tile;
	extern Graphics::TextureAtlasEntry grass1;
	extern Graphics::TextureAtlasEntry grass2;
	extern Graphics::TextureAtlasEntry grass3;
	extern Graphics::TextureAtlasEntry grass4;
	extern Graphics::TextureAtlasEntry boulder;
	extern Graphics::TextureAtlasEntry oaktree;
	extern Graphics::TextureAtlasEntry pinetree;
	extern Graphics::TextureAtlasEntry player;
	extern Graphics::TextureAtlasEntry white;

	void LoadAssets();

	inline const Graphics::TextureAtlasEntry& RandomEntityTexture()
	{
		float random = Random::Float();

		if (random > 0.50f)
			return oaktree;
		else if (random > 0.10f)
			return pinetree;
		else
			return boulder;
	}
	inline const Graphics::TextureAtlasEntry &RandomGrassTexture()
	{
		float random = Random::Float();

		if (random > 0.75f)
			return grass1;
		else if (random > 0.50f)
			return grass2;
		else if (random > 0.25f)
			return grass3;
		else
			return grass4;
	}

	struct Monster
	{
		Ref<Audio::IAudioPlayer> soundPlayer;
		
		Monster(const glm::vec3 &pos)
			: soundPlayer(Audio::IAudioPlayer::Create(monster_soundfx))
		{ 
			soundPlayer->SetPosition(pos);
			soundPlayer->Play();
		}
	};

	struct Entity
	{
		Graphics::TextureAtlasEntry texture;
		
		glm::vec2 pos;
		glm::vec2 size;

		Entity(const Graphics::TextureAtlasEntry &texture, const glm::vec2 &pos, const glm::vec2 &size)
			: texture(texture), pos(pos), size(size)
		{}
	};

	struct Tile
	{
		Graphics::TextureAtlasEntry texture;
		
		glm::vec2 pos;
		glm::vec2 size;
		
		Tile(const Graphics::TextureAtlasEntry &texture, const glm::vec2 &pos, const glm::vec2 &size)
			: texture(texture), pos(pos), size(size)
		{
			GenerateEntities();
		}

		void OnRender()
		{
			glm::vec3 pos = glm::vec3(this->pos, 1.0f);
			
			Graphics::Renderer::Draw(pos, size, texture);

			for (Entity &entity : entities)
			{
				pos = glm::vec3(entity.pos, 720.0f);
				Graphics::Renderer::Draw(pos, entity.size, entity.texture);
			}
		}

	private:
		std::vector<Entity> entities;

	private:
		void GenerateEntities()
		{
			entities.reserve(Random::Integer() % 2);

			for (int i = 0; i < entities.capacity(); ++i)
			{
				const Graphics::TextureAtlasEntry &texture = RandomEntityTexture();

				glm::vec2 size = glm::vec2{ texture.width * atlas->GetWidth(), texture.height * atlas->GetHeight() } * 1.5f;
				glm::vec2 pos{ this->pos.x, this->pos.y - size.y / 2 };

				entities.emplace_back(texture, pos, size);
			}
		}

	};
}