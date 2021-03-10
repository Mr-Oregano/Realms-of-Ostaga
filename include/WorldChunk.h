#pragma once

#include <entt/entt.hpp>

#include <Ref.h>
#include <TerrainMesh.h>

namespace Ostaga {

	struct WorldChunk
	{
		Ref<TerrainMesh> mesh;
		Ref<entt::registry> entities;

		static const int CHUNK_SIZE;
	};
}
