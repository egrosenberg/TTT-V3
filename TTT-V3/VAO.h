#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
	GLuint m_ID;
	VAO();

	void LinkVBO(VBO *vbo, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();

	virtual ~VAO();
};

#endif