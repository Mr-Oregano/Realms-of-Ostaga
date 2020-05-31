#pragma once

#include "Ostaga.h"

#include <layers/Layer.h>

#include <glad/glad.h>

namespace Ostaga {

	class TestingLayer : public Layer
	{
		GLuint vao = 0;
		GLuint vbo = 0;

	public:
		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_DEBUG_WRAP(TestingLayer() : Layer("TestingLayer") {})

		virtual void OnStart()
		{
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

		virtual void OnUpdate()
		{
		}
		virtual void OnRender()
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		virtual bool OnEvent(Event &e)
		{
			return false;
		}
	};
}
