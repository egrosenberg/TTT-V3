#ifndef FBO_H
#define FBO_H

#include <iostream>
#include <glad/glad.h>
#include "TTT.h"

class FBO
{
private:
	GLuint m_ID;
	GLuint m_Tex;
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_Samples;

	void TexGen();
	void TexGenMultisample();
	void TexGenDepthonly();
public:
	FBO(unsigned int width, unsigned int height, TTTenum fboType, unsigned int samples = 0);

	void Bind(GLenum target = GL_FRAMEBUFFER);
	void Unbind();
	void BindTex();
	void UnbindTex();

	static void CheckStatus(const char *name);

	virtual ~FBO();
};

#endif