
// Precompiled
#include "ospch.h"
//

#include <BatchGroup.h>

namespace Ostaga { namespace Graphics {

	BatchGroup::BatchGroup(const Ref<TextureAtlas> &atlas)
		: m_Atlas(atlas)
	{}

	void BatchGroup::Bind()
	{
		m_Atlas->Bind();
		glBindVertexArray(m_VaoID);
	}

	void BatchGroup::Draw()
	{
		glDrawArrays(GL_POINTS, 0, (GLsizei)(m_Mesh.size() * sizeof(Vertex)));
	}

	void BatchGroup::PushEntity(Entity entity)
	{
		m_Mesh.push_back({
			{ entity.x, entity.y, entity.width, entity.height },
			{ entity.texture.x, entity.texture.y, entity.texture.width, entity.texture.height },
			{ m_Atlas->GetWidth(), m_Atlas->GetHeight() },
			0
		});
	}

	void BatchGroup::GenerateMesh()
	{
		std::sort(m_Mesh.begin(), m_Mesh.end(), [](Vertex &v1, Vertex &v2) -> bool
		{
			return (v1.dimensions.y + v1.dimensions.w / 2) < (v2.dimensions.y + v2.dimensions.w / 2);
		});

		glCreateBuffers(1, &m_VertexBufferID);
		glNamedBufferStorage(m_VertexBufferID, m_Mesh.size() * sizeof(Vertex), m_Mesh.data(), GL_DYNAMIC_STORAGE_BIT);

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
} }