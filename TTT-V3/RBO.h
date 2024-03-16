#ifndef RBO_H
#define RBO_H

#include <glad/glad.h>

class RBO
{
private:
	GLuint m_ID;
public:
	RBO(unsigned int width, unsigned int height, unsigned int samples);

	void Bind();
	void Unbind();

	virtual ~RBO();
};

#endif