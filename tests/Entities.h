#pragma once

#include <AudioDevice.h>
#include <IAudioPlayer.h>

#include <TextureAtlas.h>
#include <Renderer.h>

#include <Maths.h>
#include <Random.h>

namespace Ostaga {
	
	// TODO: Look into using an ECS

	Ref<Graphics::TextureAtlas> atlas;
	Graphics::TextureAtlasEntry forest_tile;
	Graphics::TextureAtlasEntry grass1;
	Graphics::TextureAtlasEntry grass2;
	Graphics::TextureAtlasEntry grass3;
	Graphics::TextureAtlasEntry grass4;
	Graphics::TextureAtlasEntry boulder;
	Graphics::TextureAtlasEntry oaktree;
	Graphics::TextureAtlasEntry pinetree;
	Graphics::TextureAtlasEntry player;
	Graphics::TextureAtlasEntry white;

	void LoadAssets()
	{
		Random::SetSeed(1);
		atlas = Graphics::TextureAtlas::Create("res/textures/atlas.png");

		forest_tile = atlas->AddEntry({ 1, 1, 16, 16 });
		oaktree =	  atlas->AddEntry({ 32, 0, 64, 64 });
		pinetree =    atlas->AddEntry({ 96, 0, 64, 64 });
		grass1 =      atlas->AddEntry({ 160, 0, 16, 16 });
		grass2 =      atlas->AddEntry({ 160, 16, 16, 16 });
		grass3 =      atlas->AddEntry({ 176, 0, 16, 16 });
		grass4 =      atlas->AddEntry({ 176, 16, 16, 16 });
		boulder =     atlas->AddEntry({ 192, 0, 64, 64 });
		player =      atlas->AddEntry({ 0, 64, 32, 48 });
		white =       atlas->AddEntry({ atlas->GetWidth() - 1, atlas->GetHeight() - 1, 1, 1 });

		Graphics::Renderer::SetTextureAtlas(atlas);
	}

	const Graphics::TextureAtlasEntry& RandomEntityTexture()
	{
		float random = Random::Float();

		if (random > 0.50f)
			return oaktree;
		else if (random > 0.10f)
			return pinetree;
		else
			return boulder;
	}
	const Graphics::TextureAtlasEntry &RandomGrassTexture()
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
		Ref<Audio::IAudioPlayer> sound;
		
		// TODO: Each 'monster' should have a different source sound
		//		 figure out a way to do this (probably separate audio buffer and audio source)
		//

		Monster(const glm::vec3 &pos)
			: sound(Audio::IAudioPlayer::LoadFromFile("res/sounds/mnstr2.wav")) 
		{ 
			sound->SetPosition(pos);
			sound->Play(); 
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