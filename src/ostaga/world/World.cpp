
// Precompiled
#include "ospch.h"
//

#include <World.h>
#include <Debug.h>
#include <Random.h>

#include <Renderer.h>

namespace Ostaga {

	using namespace Graphics;

	const int WORLD_SIZE = 20;
	const int World::CHUNK_SIZE = 16;

	// TODO: Temporary assets, should create an Asset Manager
	static Ref<TextureAtlas> atlas;
	static TextureAtlasEntry grass_tile;
	static TextureAtlasEntry pine_tile;
	static TextureAtlasEntry oaktree;
	static TextureAtlasEntry pinetree;
	static TextureAtlasEntry grass1;
	static TextureAtlasEntry grass2;
	static TextureAtlasEntry grass3;
	static TextureAtlasEntry grass4;
	static TextureAtlasEntry boulder;
	//

	World::World()
		: m_CameraControl(OrthoCamera(-1280/2, 1280/2, -720/2, 720/2))
	{
		m_Shader = Shader::LoadFromFiles(
			"res/shaders/terrain-shader-vert.glsl",
			"res/shaders/terrain-shader-geom.glsl",
			"res/shaders/terrain-shader-frag.glsl"
		);

		atlas = Graphics::TextureAtlas::Create("res/textures/atlas.png");
		grass_tile = atlas->AddEntry({ 2, 66, 32, 32 });
		pine_tile = atlas->AddEntry({ 38, 66, 32, 32 });
		oaktree = atlas->AddEntry({ 32, 0, 64, 64 });
		pinetree = atlas->AddEntry({ 96, 0, 64, 64 });
		grass1 = atlas->AddEntry({ 160, 0, 16, 16 });
		grass2 = atlas->AddEntry({ 160, 16, 16, 16 });
		grass3 = atlas->AddEntry({ 176, 0, 16, 16 });
		grass4 = atlas->AddEntry({ 176, 16, 16, 16 });
		boulder = atlas->AddEntry({ 192, 0, 64, 64 });

		for (int i = 0; i < WORLD_SIZE * WORLD_SIZE; ++i)
		{
			int ChunkX = i % WORLD_SIZE;
			int ChunkY = i / WORLD_SIZE;

			GenerateChunk(ChunkX, ChunkY, 0);
		}
	}

	float TerrainGenerator(float x, float y, int seed)
	{
		x /= 10;
		y /= 10;
		Random::SetSeed(seed);

		float output_noise = 0.0f;
		float frequency = 1.0f;
		float amplitude = 1.0f;
		float divisor = 0.0f;

		for (int i = 0; i < 50; ++i)
		{
			divisor += amplitude;

			Random::SetSeed(seed + i);
			output_noise += amplitude * Random::Simplex(x * frequency, y * frequency);

			frequency *= 2.0f;
			amplitude *= 0.5f;
		}

		return output_noise / divisor;
	}

	void World::GenerateChunk(int chunkX, int chunkY, int seed)
	{
		auto chunk = CreateRef<TerrainChunk>(atlas);
		auto entities = CreateRef<BatchGroup>(atlas);

		chunkX *= CHUNK_SIZE;
		chunkY *= CHUNK_SIZE;

		std::vector<float> noiseMap;
		noiseMap.reserve(CHUNK_SIZE * CHUNK_SIZE);

		for (int y = chunkY; y < chunkY + CHUNK_SIZE; ++y)
		{
			for (int x = chunkX; x < chunkX + CHUNK_SIZE; ++x)
			{
				Tile tile = {};
				tile.x = (float) x * Tile::TILE_SIZE;
				tile.y = (float) y * Tile::TILE_SIZE;

				float noise = TerrainGenerator(x * 0.05f, y * 0.05f, seed);
				noiseMap.push_back(noise);
				if (noise > 0.5f) // Grass land
					tile.texture = grass_tile;
				else // Pine forest
					tile.texture = pine_tile;

				chunk->PushTile(tile);
			}
		}

		Random::SetSeed((unsigned int) (chunkX + chunkY + seed + 1));
		for (int i = 0; i < 10; ++i)
		{
			float offsetX = Random::Float() * Tile::TILE_SIZE * CHUNK_SIZE;
			float offsetY = Random::Float() * Tile::TILE_SIZE * CHUNK_SIZE;

			Entity e = {};
			e.x = (float) chunkX * Tile::TILE_SIZE + offsetX;
			e.y = (float) chunkY * Tile::TILE_SIZE + offsetY;

			int tileX = (int) (offsetX / Tile::TILE_SIZE);
			int tileY = (int) (offsetY / Tile::TILE_SIZE);

			if (noiseMap[tileX + tileY * CHUNK_SIZE] > 0.5f) // Grass land
				e.texture = oaktree;

			e.width = e.texture.width * atlas->GetWidth() * 2;
			e.height = e.texture.height * atlas->GetHeight() * 2;

			entities->PushEntity(e);
		}

		for (int i = 0; i < 150; ++i)
		{
			float offsetX = Random::Float() * Tile::TILE_SIZE * CHUNK_SIZE;
			float offsetY = Random::Float() * Tile::TILE_SIZE * CHUNK_SIZE;

			Entity e = {};
			e.x = (float)chunkX * Tile::TILE_SIZE + offsetX;
			e.y = (float)chunkY * Tile::TILE_SIZE + offsetY;

			int tileX = (int)(offsetX / Tile::TILE_SIZE);
			int tileY = (int)(offsetY / Tile::TILE_SIZE);

			if (noiseMap[tileX + tileY * CHUNK_SIZE] <= 0.5f) // Pine forest
			{
				if (Random::Float() < 0.9f)
					e.texture = pinetree;
				else
					e.texture = boulder;
			}

			e.width = e.texture.width * atlas->GetWidth() * 2;
			e.height = e.texture.height * atlas->GetHeight() * 2;

			entities->PushEntity(e);
		}

		for (int i = 0; i < 1000; ++i)
		{
			float offsetX = Random::Float() * Tile::TILE_SIZE * CHUNK_SIZE;
			float offsetY = Random::Float() * Tile::TILE_SIZE * CHUNK_SIZE;

			int tileX = (int)(offsetX / Tile::TILE_SIZE);
			int tileY = (int)(offsetY / Tile::TILE_SIZE);

			if (noiseMap[tileX + tileY * CHUNK_SIZE] <= 0.5f)
				continue;

			Entity e = {};
			e.x = (float)chunkX * Tile::TILE_SIZE + offsetX;
			e.y = (float)chunkY * Tile::TILE_SIZE + offsetY;

			switch (Random::Integer() % 4)
			{
			case 0: e.texture = grass1; break;
			case 1: e.texture = grass2; break;
			case 2: e.texture = grass3; break;
			case 3: e.texture = grass4; break;
			}

			e.width = e.texture.width * atlas->GetWidth() * 2;
			e.height = e.texture.height * atlas->GetHeight() * 2;

			entities->PushEntity(e);
		}

		chunk->GenerateMesh();
		m_Chunks.push_back(chunk);

		entities->GenerateMesh();
		m_EntityChunks.push_back(entities);
	}

	void World::OnRender()
	{
		m_Shader->Bind();
		m_Shader->SetUniformMat4("u_ViewProjection", m_CameraControl.GetCamera().GetViewProj());
		
		glm::vec3 &center = m_CameraControl.GetPosition();
		int left = (int) (center.x - 1500/2.f) / (Tile::TILE_SIZE * CHUNK_SIZE);
		int right = (int) (center.x + 1500/2.f) / (Tile::TILE_SIZE * CHUNK_SIZE);
		int top = (int) (center.y - 1000/2.f) / (Tile::TILE_SIZE * CHUNK_SIZE);
		int bottom = (int) (center.y + 1000/2.f) / (Tile::TILE_SIZE * CHUNK_SIZE);

		for (int ChunkX = left; ChunkX <= right + 1; ++ChunkX)
		{
			for (int ChunkY = top; ChunkY <= bottom + 1; ++ChunkY)
			{
				int index = ChunkX + ChunkY * WORLD_SIZE;
				if (index < 0 || index >= m_Chunks.size())
					continue;

				TerrainChunk &chunk = *m_Chunks[index];
				Renderer::Draw(chunk);
			}
		}

		for (int ChunkX = left; ChunkX <= right + 1; ++ChunkX)
		{
			for (int ChunkY = top; ChunkY <= bottom + 1; ++ChunkY)
			{
				int index = ChunkX + ChunkY * WORLD_SIZE;
				if (index < 0 || index >= m_Chunks.size())
					continue;

				BatchGroup &chunk = *m_EntityChunks[index];
				Renderer::Draw(chunk);
			}
		}
	}

	void World::OnUpdate(TimeStep ts)
	{
		m_CameraControl.OnUpdate(ts);
	}

	void World::OnEvent(Event &e)
	{
		m_CameraControl.OnEvent(e);
	}

	void World::OnGui()
	{
		ImGui::Begin("World");

		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
			ImGui::InputFloat3("Position", glm::value_ptr(m_CameraControl.GetPosition()), 3);

		ImGui::End();
	}
}


