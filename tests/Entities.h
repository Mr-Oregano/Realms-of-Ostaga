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
		}
	};
}