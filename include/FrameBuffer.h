#pragma once

#include <Ref.h>

#include <glad/glad.h>

namespace Ostaga { namespace Graphics {

	struct FrameBufferProps
	{
		unsigned int width;
		unsigned int height;
		unsigned int samples = 1;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(const FrameBufferProps &props);
		~FrameBuffer();

		void Bind();
		void Unbind();

		void Resize(unsigned int width, unsigned int height);

		inline GLuint GetColorAttachmentID() const { return m_ColorAttachment; }
		inline GLuint GetContextID() const { return m_ContextID; }
	private:
		void GenerateAttachments();

	public:
		static Ref<FrameBuffer> Create(const FrameBufferProps &props);

	private:
		GLuint m_ContextID = 0;
		GLuint m_ColorAttachment = 0;
		GLuint m_DepthAttachment = 0;
		FrameBufferProps m_Props;
	};

} }