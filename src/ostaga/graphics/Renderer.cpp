
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
		glm::vec2 texelSize;
		float texID;
		float rotation;
	};

	struct RendererData
	{
		Ref<Shader> shader;
		Ref<Texture> whiteTex;

		GLuint VAO = 0;
		GLuint VBO = 0;
	
		std::vector<Vertex> vertices;
		size_t vertexCount = 0;

		std::vector<Ref<Texture>> textures;
		size_t textureCount = 0;
	};

	static RendererData *renderer = nullptr;

	static int MAX_TEXTURE_UNITS = 1;

	void Renderer::Init(const RendererProps &props)
	{
		renderer = new RendererData;

		renderer->shader = Shader::LoadFromFiles(
			"res/shaders/renderer-shader-vert.glsl",
			"res/shaders/renderer-shader-geom.glsl",
			"res/shaders/renderer-shader-frag.glsl"
		);
		renderer->shader->Bind();

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
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, texID));
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, texelSize));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		// Renderer State
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		// Graphics device polling
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_UNITS);
		//
		
		unsigned char data[] = { 0xff, 0xff, 0xff, 0xff };
		renderer->whiteTex = Texture::LoadFromData(data, 1, 1, 4, { Filter::Nearest, WrapMode::ClampToEdge, MipmapMode::None });

		renderer->textures.resize(MAX_TEXTURE_UNITS);

		std::vector<int> texUnits(MAX_TEXTURE_UNITS);
		for (int i = 0; i < texUnits.size(); ++i)
			texUnits[i] = i;

		renderer->shader->SetUniformIntV("u_Textures", texUnits.data(), (GLsizei) texUnits.size());
		
		// Bindings currently remain constant throughout the application
		// no need to rebind for now.

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
		Shader &shader = *renderer->shader;
		shader.SetUniformMat4("u_ViewProjection", camera);
	}

	void Renderer::Draw(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, float rotation)
	{
		Draw(position, size, color, renderer->whiteTex, rotation);
	}

	void Renderer::Draw(const glm::vec2 &position, const glm::vec2 &size, const Ref<Assets::Texture> &texture, float rotation)
	{
		Draw(position, size, glm::vec4{ 1 }, texture, rotation);
	}

	void Renderer::Draw(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const Ref<Texture> &texture, float rotation)
	{
		auto &vertices = renderer->vertices;
		auto &vertexCount = renderer->vertexCount;

		auto &textures = renderer->textures;
		auto &textureCount = renderer->textureCount;

		int texIndex = 0;
		for (; texIndex < textureCount; ++texIndex)
			if (texture == textures[texIndex])
				break;

		if (vertexCount >= vertices.capacity() || texIndex >= textures.capacity())
		{
			Flush();
			texIndex = 0;
		}

		if (texIndex >= textureCount)
			textures[textureCount++] = texture;

		Vertex &v = vertices[vertexCount];
		v.dimensions = { position.x, position.y, size.x, size.y };
		v.rotation = rotation;
		v.color = color;
		v.texID = (float) texIndex;
		v.texelSize = { texture->GetWidth(), texture->GetHeight() };
		++vertexCount;
	}

	void Renderer::EndScene()
	{
		if (renderer->vertexCount > 0)
			Flush();
	}

	void Renderer::Flush()
	{
		auto &vertices = renderer->vertices;
		auto &vertexCount = renderer->vertexCount;

		auto &textures = renderer->textures;
		auto &textureCount = renderer->textureCount;

		for (int i = 0; i < textureCount; ++i)
			textures[i]->Bind(i);

		glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr) (vertexCount * sizeof(Vertex)), vertices.data());
		glDrawArrays(GL_POINTS, 0, (GLsizei) vertexCount);
		vertexCount = 0;
		textureCount = 0;
	}

} }


