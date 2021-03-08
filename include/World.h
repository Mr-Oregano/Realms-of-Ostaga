#pragma once

#include <Ostaga.h>

#include <Ref.h>
#include <Event.h>
#include <TimeStep.h>

#include <Shader.h>
#include <CameraController.h>
#include <TerrainChunk.h>
#include <BatchGroup.h>

#include <Maths.h>

#include <vector>

namespace Ostaga {

	class World
	{
	public:
		World();

		void OnRender();
		void OnUpdate(TimeStep ts);
		void OnEvent(Event &e);
		void OnGui();

	private:
		void GenerateChunk(int chunkX, int chunkY, int seed);

	private:
		std::vector<Ref<TerrainChunk>> m_Chunks;
		std::vector<Ref<Graphics::BatchGroup>> m_EntityChunks;
		Ref<Graphics::Shader> m_Shader;
		CameraController m_CameraControl;

	private:
		static const int CHUNK_SIZE;
	};
}