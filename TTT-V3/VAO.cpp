#include "VAO.h"

VAO::VAO()
{
	// generate our vertex array
	glGenVertexArrays(1, &m_ID);
}

void VAO::LinkAttrib(VBO* vbo, GLuint layout, GLuint componentN, GLuint type, GLsizeiptr stride, void* offset)
{
	vbo->Bind();

	glVertexAttribPointer(layout, componentN, type, GL_FALSE, stride, offset);

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
