
// Precompiled
#include "ospch.h"
//

#include "Renderer.h"

#include <assets/Shader.h>
#include <Ostaga.h>

#include <glad/glad.h>


namespace Ostaga { namespace Graphics {

	using namespace Assets;

	struct RendererData
	{
		Ref<Shader> shader;

		GLuint vertexArray = 0;
		GLuint vertexData = 0;
	};

	static RendererData *data = nullptr;

	void Renderer::Init()
	{
		data = new RendererData;

		data->shader = Shader::LoadFromFiles(
			"res/shaders/renderer-shader-vert.glsl",
			"res/shaders/renderer-shader-frag.glsl"
		);
		data->shader->Bind();

		GLuint &vertexArray = data->vertexArray;
		GLuint &vertexData = data->vertexData;

		float QUAD_VERTICES[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,

			 0.5f,  0.5f,
			-0.5f,  0.5f,
			-0.5f, -0.5f
		};

		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexData);
		glBindBuffer(GL_ARRAY_BUFFER, vertexData);
		glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

	}

	void Renderer::Shutdown()
	{
		delete data;
		data = nullptr;
	}

	void Renderer::BeginScene(const glm::mat4& camera)
	{
		const Shader &shader = *data->shader;
		shader.SetUniformMat4("u_ViewProjection", camera);
	}

	void Renderer::Draw(const glm::vec2 &position, const glm::vec2 &size, float rotation)
	{
		glm::mat4 model = glm::translate(glm::mat4{1.0f}, glm::vec3{position, 0.0f})
						* glm::scale(glm::mat4{1.0f}, glm::vec3{size, 0.0f})
						* glm::rotate(glm::mat4{1.0f}, glm::radians(rotation), { 0, 0, 1 });

		data->shader->SetUniformMat4("u_Model", model);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Renderer::EndScene()
	{
	
	}

} }


