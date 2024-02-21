#include "VAO.h"

VAO::VAO()
{
	// generate our vertex array
	glGenVertexArrays(1, &m_ID);
}

void VAO::LinkVBO(VBO *vbo, GLuint layout)
{
	vbo->Bind();

	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// enble vertex attribute array
	glEnableVertexAttribArray(layout);

	vbo->Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(m_ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &m_ID);
}

VAO::~VAO()
{
	Delete();
}