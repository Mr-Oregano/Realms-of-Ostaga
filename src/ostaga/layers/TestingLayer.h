#pragma once

#include <Ostaga.h>
#include <Util.h>

#include <layers/Layer.h>
#include <assets/Shader.h>

#include <glad/glad.h>

namespace Ostaga {

	class TestingLayer : public Layer
	{
		GLuint vao = 0;
		GLuint vbo = 0;

		Ref<Assets::Shader> m_Shader;

	public:
		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_DEBUG_WRAP(TestingLayer() : Layer("TestingLayer") {})

		virtual void OnStart()
		{
			m_Shader = Assets::Shader::LoadFromFiles(
				"res/shaders/test-vertex.glsl",
				"res/shaders/test-fragment.glsl"
			);

			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_Projection", glm::ortho(-16.0f/9.0f, 16.0f/9.0f, -1.0f, 1.0f));

			float vertices[] = {
				-0.5f, -0.5f,
				 0.0f,  0.5f,
				 0.5f, -0.5f
			};

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
		}
		virtual void OnStop()
		{
		}

		virtual void OnUpdate(TimeStep ts)
		{
			static float angle = 0.0f;
			m_Shader->SetUniformMat4("u_Model", glm::rotate(glm::mat4(1.0), glm::radians(angle), { 0, 0, 1}));
			angle += ts * 100.0f;
		}
		virtual void OnRender()
		{
			m_Shader->SetUniformFloat4("u_Color", { 1.0f, 0.0f, 1.0f, 1.0f });
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		virtual bool OnEvent(Event &e)
		{
			return false;
		}
	};
}
