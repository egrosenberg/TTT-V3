#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>

class EBO
{
public:
	GLuint m_ID;
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();

	virtual ~EBO();
};

#endif