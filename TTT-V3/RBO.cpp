#include "RBO.h"

RBO::RBO(unsigned int width, unsigned int height)
{
	glGenRenderbuffers(1, &m_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
	// configured RBO storage          // store stencil and depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
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