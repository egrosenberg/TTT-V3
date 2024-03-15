#ifndef FBO_H
#define FBO_H

#include <iostream>
#include <glad/glad.h>

class FBO
{
private:
	GLuint m_ID;
	GLuint m_Tex;
	unsigned int m_Width;
	unsigned int m_Height;
	bool m_MSAA;

	void TexGen();
	void TexGenMultisample();
public:
	FBO(unsigned int width, unsigned int height, bool multisample);

	void Bind(GLenum target = GL_FRAMEBUFFER);
	void Unbind();
	void BindTex();
	void UnbindTex();

	virtual ~FBO();
};

#endif