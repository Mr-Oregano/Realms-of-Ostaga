
// Precompiled
#include "ospch.h"
//

#include "FrameBuffer.h"

#include <Ostaga.h>

namespace Ostaga { namespace Graphics {

	FrameBuffer::FrameBuffer(const FrameBufferProps &props)
		: m_Props(props)
	{
		glCreateFramebuffers(1, &m_ContextID);
		GenerateAttachments();
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_ContextID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
		m_ContextID = 0;
	}

	void FrameBuffer::GenerateAttachments()
	{
		if (m_ContextID)
		{
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_ContextID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Props.width, m_Props.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Props.width, m_Props.height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		OSTAGA_IF_DEBUG(
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				LOG_ERROR("Framebuffer is incomplete!");
		)

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ContextID);
		glViewport(0, 0, m_Props.width, m_Props.height);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Resize(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, m_Props.width, m_Props.height);
		m_Props.width = width;
		m_Props.height = height;

		GenerateAttachments();
		glBindFramebuffer(GL_FRAMEBUFFER, m_ContextID);
	}

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferProps &props)
	{
		return CreateRef<FrameBuffer>(props);
	}

} }