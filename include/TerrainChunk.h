#pragma once

#include <Geometry.h>

#include <Ref.h>
#include <TextureAtlas.h>
#include <Shader.h>

#include <vector>

#include <glad/glad.h>

namespace Ostaga {

	struct Tile
	{
		float x;
		float y;
		Graphics::TextureAtlasEntry texture;

		static const int TILE_SIZE;
	};

	class TerrainChunk : public Graphics::Geometry
	{
	private:
		struct Vertex
		{
			glm::vec4 dimensions;
			glm::vec4 texCoords;
			glm::vec2 texelSize;
		};

	public:
		TerrainChunk(const Ref<Graphics::TextureAtlas> &atlas);

		virtual void Bind() override;
		virtual void Draw() override;
		
		void PushTile(Tile tile);
		void GenerateMesh();

	private:
		GLuint m_VaoID = 0;
		GLuint m_VertexBufferID = 0;

		std::vector<Vertex> m_ChunkMesh;
		Ref<Graphics::TextureAtlas> m_Atlas;
	};
}