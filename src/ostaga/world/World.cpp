
// Precompiled
#include "ospch.h"
//

#include <World.h>
#include <Debug.h>
#include <Random.h>

#include <Renderer.h>

namespace Ostaga {

	using namespace Graphics;

	static entt::entity player;

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
	static TextureAtlasEntry player_tex;
	static Ref<Graphics::Shader> shader;
	
	static void LoadAssets()
	{
		shader = Shader::LoadFromFiles(
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
		player_tex = atlas->AddEntry({ 2, 2, 29, 45 });
	}
	//

	World::World(int width, int height, int seed)
		: m_Width(width), m_Height(height), m_Seed(seed)
	{
		PROFILE_FUNCTION();
		// Temporary
		LoadAssets();
		//

		glm::vec2 worldCenter = { width / 2.f, height / 2.f };
		worldCenter = ChunkToTileCoords(worldCenter.x, worldCenter.y);
		worldCenter = TileToXYCoords(worldCenter.x, worldCenter.y);

		ControllerProps props;
		props.speed = 45.0f;
		props.x = worldCenter.x;
		props.y = worldCenter.y;

		OrthoCamera camera = OrthoCamera(-1280 / 4, 1280 / 4, -720 / 4, 720 / 4);
		m_CameraControl = CameraController(camera, props);

		m_Scene = CreateRef<Scene>(camera);
		m_Scene->atlas = atlas;

		// Player creation: Temporary
		player = m_Scene->entities.create();
		glm::vec2 view = m_CameraControl.GetPosition();
		glm::vec2 size = { player_tex.width * atlas->GetWidth(), player_tex.height * atlas->GetHeight() };
		m_Scene->entities.emplace<CTransform>(player, glm::vec2{ view.x, view.y }, size, 0.f);
		m_Scene->entities.emplace<CRenderable>(player, player_tex);
		//

		m_Chunks.reserve(width * height);
		for (int i = 0; i < width * height; ++i)
		{
			int ChunkX = i % width;
			int ChunkY = i / width;

			m_Chunks.push_back(GenerateChunk(ChunkX, ChunkY, seed));
		}
	}

	float TerrainGenerator(float x, float y, int seed)
	{
		PROFILE_FUNCTION();
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

	WorldChunk World::GenerateChunk(int chunkX, int chunkY, int seed)
	{
		PROFILE_FUNCTION();
		static WorldChunk chunk;

		chunk.mesh = CreateRef<TerrainMesh>(atlas);
		chunk.entities = CreateRef<entt::registry>();

		glm::ivec2 tile = (glm::ivec2) ChunkToTileCoords((float) chunkX, (float) chunkY);

		// Temporary
		std::vector<float> noiseMap;
		//

		for (int y = tile.y; y < tile.y + WorldChunk::CHUNK_SIZE; ++y)
		{
			for (int x = tile.x; x < tile.x + WorldChunk::CHUNK_SIZE; ++x)
			{
				Tile t;
				t.x = (float) x * Tile::TILE_SIZE;
				t.y = (float) y * Tile::TILE_SIZE;

				// Temporary - TODO: Biome generation system
				float noise = TerrainGenerator(x * 0.05f, y * 0.05f, seed);
				noiseMap.push_back(noise);
				t.texture = noise > 0.5f ? grass_tile : pine_tile;
				//

				chunk.mesh->PushTile(t);
			}
		}

		chunk.mesh->GenerateMesh();

		// Temporary
		Random::SetSeed((unsigned int) (chunkX + chunkY + seed + 1));
		//

		for (int i = 0; i < 50; ++i)
		{
			entt::entity id = chunk.entities->create();

			// Temporary - Will be implemented in biome generation system
			glm::vec2 inChunk = { Random::Float(), Random::Float() };
			glm::ivec2 tile = (glm::ivec2) ChunkToTileCoords(inChunk.x, inChunk.y);
			glm::vec2 pos = ChunkToTileCoords(chunkX + inChunk.x, chunkY + inChunk.y);
			pos = TileToXYCoords(pos.x, pos.y);
			//

			TextureAtlasEntry texture;

			if (noiseMap[tile.x + tile.y * WorldChunk::CHUNK_SIZE] > 0.5f) // Grass land
				texture = oaktree;
			else
				texture = pinetree;

			float width = texture.width * atlas->GetWidth();
			float height = texture.height * atlas->GetHeight();
			glm::vec2 size = { width, height };

			chunk.entities->emplace<CTransform>(id, pos, size, 0.f);
			chunk.entities->emplace<CRenderable>(id, texture);
		}

		return chunk;
	}

	glm::vec2 World::ChunkToTileCoords(float chunkX, float chunkY)
	{
		return { chunkX * WorldChunk::CHUNK_SIZE, chunkY * WorldChunk::CHUNK_SIZE };
	}

	glm::vec2 World::TileToXYCoords(float tileX, float tileY)
	{
		return { tileX * Tile::TILE_SIZE, tileY * Tile::TILE_SIZE };
	}

	glm::vec2 World::XYToTileCoords(float x, float y)
	{
		return { x / Tile::TILE_SIZE, y / Tile::TILE_SIZE };
	}

	glm::vec2 World::TileToChunkCoords(float tileX, float tileY)
	{
		return { tileX / WorldChunk::CHUNK_SIZE, tileY / WorldChunk::CHUNK_SIZE };
	}

	void World::OnRender()
	{
		PROFILE_FUNCTION();
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", m_CameraControl.GetCamera().GetViewProj());
		m_Scene->camera = m_CameraControl.GetCamera();
		
		glm::vec3 &center = m_CameraControl.GetPosition();
		int left = (int) (center.x - 1500/2.f) / (Tile::TILE_SIZE * WorldChunk::CHUNK_SIZE);
		int right = (int) (center.x + 1500/2.f) / (Tile::TILE_SIZE * WorldChunk::CHUNK_SIZE);
		int top = (int) (center.y - 1000/2.f) / (Tile::TILE_SIZE * WorldChunk::CHUNK_SIZE);
		int bottom = (int) (center.y + 1000/2.f) / (Tile::TILE_SIZE * WorldChunk::CHUNK_SIZE);

		for (int ChunkX = left; ChunkX <= right + 1; ++ChunkX)
		{
			for (int ChunkY = top; ChunkY <= bottom + 1; ++ChunkY)
			{
				int index = ChunkX + ChunkY * m_Width;
				if (index < 0 || index >= m_Chunks.size())
					continue;

				m_Scene->chunks.push_back(m_Chunks[index]);
			}
		}

		// Temporary
		auto& playerTransform = m_Scene->entities.get<CTransform>(player);
		playerTransform.pos = { center.x, center.y };
		//

		Renderer::Draw(*m_Scene);

		// Temporary
		m_Scene->chunks.clear();
		//
	}

	void World::OnUpdate(TimeStep ts)
	{
		PROFILE_FUNCTION();
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


