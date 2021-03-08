
// Precompiled
#include "ospch.h"
//

#define WIREFRAME_MODE 0
#include "Renderer.h"

#include <glad/glad.h>

namespace Ostaga { namespace Graphics {

	struct Vertex
	{
		glm::vec4 dimensions;
		glm::vec4 tint;
		glm::vec4 texCoords;
		glm::vec2 texelSize;
		float rotation;
	};

	struct RendererData
	{
		Ref<Shader> shader;
		Ref<TextureAtlas> atlas;

		GLuint VAO = 0;
		GLuint VBO = 0;
	
		std::vector<Vertex> vertices;
		size_t vertexCount = 0;
	};

	static RendererData *renderer = nullptr;
	static RendererAnalytics analytics = {};

	void SetPipelineState()
	{
		// Renderer State
#if WIREFRAME_MODE
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}
	void LoadAssets()
	{
		PROFILE_FUNCTION();
		renderer->shader = Shader::LoadFromFiles(
			"res/shaders/renderer-shader-vert.glsl",
			"res/shaders/renderer-shader-geom.glsl",
			"res/shaders/renderer-shader-frag.glsl"
		);
	}
	void CreateBufferStore()
	{
		PROFILE_FUNCTION();
		GLuint &VAO = renderer->VAO;
		GLuint &VBO = renderer->VBO;

		glCreateBuffers(1, &VBO);
		glNamedBufferStorage(VBO, renderer->vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

		glCreateVertexArrays(1, &VAO);
		glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));

		glVertexArrayAttribFormat(VAO, 0, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, dimensions));
		glVertexArrayAttribFormat(VAO, 1, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, rotation));
		glVertexArrayAttribFormat(VAO, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, tint));
		glVertexArrayAttribFormat(VAO, 3, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texelSize));
		glVertexArrayAttribFormat(VAO, 4, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords));

		glVertexArrayAttribBinding(VAO, 0, 0);
		glVertexArrayAttribBinding(VAO, 1, 0);
		glVertexArrayAttribBinding(VAO, 2, 0);
		glVertexArrayAttribBinding(VAO, 3, 0);
		glVertexArrayAttribBinding(VAO, 4, 0);

		glEnableVertexArrayAttrib(VAO, 0);
		glEnableVertexArrayAttrib(VAO, 1);
		glEnableVertexArrayAttrib(VAO, 2);
		glEnableVertexArrayAttrib(VAO, 3);
		glEnableVertexArrayAttrib(VAO, 4);
	}

	void Renderer::Init(const RendererProps &props)
	{
		PROFILE_FUNCTION();
		renderer = new RendererData;
		renderer->vertices.resize(props.batchCapacity);

		SetPipelineState();
		CreateBufferStore();
		LoadAssets();
	}

	void Renderer::Shutdown()
	{
		glDeleteBuffers(1, &renderer->VBO);
		glDeleteVertexArrays(1, &renderer->VAO);

		delete renderer;
		renderer = nullptr;
	}

	void Renderer::BeginScene(const glm::mat4& camera)
	{
		PROFILE_FUNCTION();
		Shader &shader = *renderer->shader;
		shader.Bind();
		shader.SetUniformMat4("u_ViewProjection", camera);

		OSTAGA_IF_DEBUG(
			analytics.drawCalls = 0;
			analytics.submissions = 0;
		)
	}

	void Renderer::SetTextureAtlas(const Ref<TextureAtlas> &atlas)
	{
		if (renderer->vertexCount > 0)
			Flush();
		renderer->atlas = atlas;
		OSTAGA_IF_DEBUG(analytics.currentAtlas = atlas;)
	}

	void Renderer::Draw(const glm::vec2 &pos, const glm::vec2 &size, const TextureAtlasEntry &tex, const glm::vec4 &tint, float rotation)
	{
		auto &vertices = renderer->vertices;
		auto &vertexCount = renderer->vertexCount;
		auto &atlas = *renderer->atlas;

		if (vertexCount >= vertices.size())
			Flush();

		Vertex &v = vertices[vertexCount];
		v.dimensions = { pos.x, pos.y, size.x, size.y };
		v.rotation = rotation;
		v.tint = tint;
		v.texelSize = { atlas.GetWidth(), atlas.GetHeight() };
		v.texCoords = { tex.x, tex.y, tex.width, tex.height };
		++vertexCount;

		OSTAGA_IF_DEBUG(++analytics.submissions;)
	}

	void Renderer::EndScene()
	{
		PROFILE_FUNCTION();
		if (renderer->vertexCount > 0)
			Flush();
	}

	void Renderer::Flush()
	{
		PROFILE_FUNCTION();
		
		GLuint &VAO = renderer->VAO;
		GLuint &VBO = renderer->VBO;
		auto &vertices = renderer->vertices;
		auto &vertexCount = renderer->vertexCount;

		renderer->atlas->Bind();
		renderer->shader->Bind();
		glBindVertexArray(VAO);

		glNamedBufferSubData(VBO, 0, (GLsizeiptr) (vertexCount * sizeof(Vertex)), vertices.data());
		glDrawArrays(GL_POINTS, 0, (GLsizei) vertexCount);
		vertexCount = 0;

		OSTAGA_IF_DEBUG(++analytics.drawCalls;)
	}

	void Renderer::Draw(Geometry &geometry)
	{
		geometry.Bind();
		geometry.Draw();
	}

	RendererAnalytics Renderer::GetAnalytics()
	{
		return analytics;
	}

} }


