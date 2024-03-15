#include "FBO.h"

FBO::FBO(unsigned int width, unsigned int height, bool multisample)
{
	m_Width = width;
	m_Height = height;
	m_MSAA = multisample;

	// initalize frame buffer
	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	if (m_MSAA)
	{
		TexGenMultisample();
	}
	else
	{
		TexGen();
	}
}

void FBO::TexGen()
{
	// create texture
	glGenTextures(1, &m_Tex);
	glBindTexture(GL_TEXTURE_2D, m_Tex);
	// configure texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// set upscaling mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Prevent edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// attach the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Tex, 0);
}

void FBO::TexGenMultisample()
{
	glGenTextures(1, &m_Tex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_Tex);
	// configure texture
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_RGB, m_Width, m_Height, GL_TRUE);
	// set upscaling mode
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Prevent edge bleeding
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// attach the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_Tex, 0);
}

void FBO::Bind(GLenum target)
{
	glBindFramebuffer(target, m_ID);
}

void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::BindTex()
{
	glBindTexture(GL_TEXTURE_2D, m_Tex);
}

void FBO::UnbindTex()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

FBO::~FBO()
{
	glDeleteTextures(1, &m_Tex);
}
