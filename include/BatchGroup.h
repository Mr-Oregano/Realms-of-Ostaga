#pragma once

#include <Ostaga.h>
#include <Geometry.h>

#include <Ref.h>
#include <TextureAtlas.h>
#include <Shader.h>

namespace Ostaga { namespace Graphics {

	struct Entity
	{
		float x;
		float y;
		float width;
		float height;
		TextureAtlasEntry texture;
	};

	class BatchGroup : public Geometry
	{
	private:
		struct Vertex
		{
			glm::vec4 dimensions;
			glm::vec4 texCoords;
			glm::vec2 texelSize;
			float rotation;
		};

	public:
		BatchGroup(const Ref<TextureAtlas> &atlas);

		virtual void Bind() override;
		virtual void Draw() override;

		void PushEntity(Entity entity);
		void GenerateMesh();

	private:
		GLuint m_VaoID = 0;
		GLuint m_VertexBufferID = 0;

		std::vector<Vertex> m_Mesh;
		Ref<TextureAtlas> m_Atlas;
	};

} }