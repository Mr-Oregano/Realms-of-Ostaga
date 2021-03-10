#pragma once

#include <Ostaga.h>

#include <Ref.h>
#include <Event.h>
#include <TimeStep.h>

#include <Shader.h>
#include <CameraController.h>
#include <WorldChunk.h>
#include <Scene.h>

#include <Maths.h>

#include <vector>

namespace Ostaga {

	class World
	{
	public:
		// Chunk width and height
		//
		World(int width, int height, int seed);

		void OnRender();
		void OnUpdate(TimeStep ts);
		void OnEvent(Event &e);
		void OnGui();

	private:
		WorldChunk GenerateChunk(int chunkX, int chunkY, int seed);

		// Utility
		glm::vec2 ChunkToTileCoords(float chunkX, float chunkY);
		glm::vec2 TileToXYCoords(float tileX, float tileY);
		glm::vec2 XYToTileCoords(float x, float y);
		glm::vec2 TileToChunkCoords(float tileX, float tileY);

	private:
		int m_Width;
		int m_Height;
		int m_Seed;
		CameraController m_CameraControl;
		
		std::vector<WorldChunk> m_Chunks;
		Ref<Scene> m_Scene;
	};
}