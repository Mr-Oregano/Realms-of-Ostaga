
// Precompiled
#include "ospch.h"
//

#include <TerrainMesh.h>

namespace Ostaga {

	using namespace Graphics;

	const int Tile::TILE_SIZE = 32;

	TerrainMesh::TerrainMesh(const Ref<TextureAtlas> &atlas)
		: m_Atlas(atlas)
	{
	}

	void TerrainMesh::Bind()
	{
		m_Atlas->Bind();
		glBindVertexArray(m_VaoID);
	}

	void TerrainMesh::Draw()
	{
		PROFILE_FUNCTION();
		glDrawArrays(GL_POINTS, 0, (GLsizei) (m_ChunkMesh.size() * sizeof(Vertex)));
	}

	void TerrainMesh::PushTile(Tile tile)
	{
		PROFILE_FUNCTION();
		m_ChunkMesh.push_back({
			{ tile.x, tile.y, Tile::TILE_SIZE, Tile::TILE_SIZE },
			{ tile.texture.x, tile.texture.y, tile.texture.width, tile.texture.height },
			{ m_Atlas->GetWidth(), m_Atlas->GetHeight() }
		});
	}

	void TerrainMesh::GenerateMesh()
	{
		PROFILE_FUNCTION();
		// TODO: Implement greedy meshing to reduce geometry

		glCreateBuffers(1, &m_VertexBufferID);
		glNamedBufferStorage(m_VertexBufferID, m_ChunkMesh.size() * sizeof(Vertex), m_ChunkMesh.data(), GL_DYNAMIC_STORAGE_BIT);

		glCreateVertexArrays(1, &m_VaoID);
		glVertexArrayVertexBuffer(m_VaoID, 0, m_VertexBufferID, 0, sizeof(Vertex));

		glVertexArrayAttribFormat(m_VaoID, 0, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, dimensions));
		glVertexArrayAttribFormat(m_VaoID, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texelSize));
		glVertexArrayAttribFormat(m_VaoID, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords));

		glVertexArrayAttribBinding(m_VaoID, 0, 0);
		glVertexArrayAttribBinding(m_VaoID, 1, 0);
		glVertexArrayAttribBinding(m_VaoID, 2, 0);

		glEnableVertexArrayAttrib(m_VaoID, 0);
		glEnableVertexArrayAttrib(m_VaoID, 1);
		glEnableVertexArrayAttrib(m_VaoID, 2);
	}
}

