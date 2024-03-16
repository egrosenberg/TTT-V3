#include "RBO.h"

RBO::RBO(unsigned int width, unsigned int height, unsigned int samples)
{
	glGenRenderbuffers(1, &m_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
	// configured RBO storage - stencil and depth buffer
	if (samples == 0)
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}
	else
	{
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	}
	// attach RBO to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ID);
}

void RBO::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

void RBO::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RBO::~RBO()
{

}