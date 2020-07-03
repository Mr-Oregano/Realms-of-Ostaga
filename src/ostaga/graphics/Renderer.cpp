
// Precompiled
#include "ospch.h"
//

#include "Renderer.h"

#include <assets/Shader.h>
#include <Ostaga.h>

#include <glad/glad.h>


namespace Ostaga { namespace Graphics {

	using namespace Assets;

	struct Vertex
	{
		glm::vec4 dimensions;
		glm::vec4 color;
		float rotation;
	};

	struct RendererData
	{
		Ref<Shader> shader;

		GLuint VAO = 0;
		GLuint VBO = 0;
	
		std::vector<Vertex> vertices;
		size_t vertexCount = 0;
	};

	static RendererData *renderer = nullptr;

	void Renderer::Init(const RendererProps &props)
	{
		renderer = new RendererData;

		renderer->shader = Shader::LoadFromFiles(
			"res/shaders/renderer-shader-vert.glsl",
			"res/shaders/renderer-shader-geom.glsl",
			"res/shaders/renderer-shader-frag.glsl"
		);

		GLuint &VAO = renderer->VAO;
		GLuint &VBO = renderer->VBO;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Buffer Data
		renderer->vertices.resize(props.batchCapacity);
		glBufferData(GL_ARRAY_BUFFER, renderer->vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, dimensions));
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, rotation));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Bindings currently remain constant throughout the application
		// no need to rebind for now.
		
		renderer->shader->Bind();
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindVertexArray(VAO);

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
		const Shader &shader = *renderer->shader;
		shader.SetUniformMat4("u_ViewProjection", camera);
	}

	void Renderer::Draw(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
	{
		std::vector<Vertex> &vertices = renderer->vertices;
		size_t &vertexCount = renderer->vertexCount;

		// If the buffer has reached maximum capacity then
		// flush before inserting new elements.
		if (vertexCount > vertices.capacity() - 1)
			Flush();

		Vertex &v = vertices[vertexCount];
		v.dimensions = { position.x, position.y, size.x, size.y };
		v.rotation = glm::radians(rotation);
		v.color = color;
		++vertexCount;
	}

	void Renderer::EndScene()
	{
		if (renderer->vertexCount > 0)
			Flush();
	}

	void Renderer::Flush()
	{
		std::vector<Vertex> &vertices = renderer->vertices;
		size_t &vertexCount = renderer->vertexCount;

		glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr) (vertexCount * sizeof(Vertex)), vertices.data());
		glDrawArrays(GL_POINTS, 0, (GLsizei) vertexCount);
		vertexCount = 0;
	}

} }


