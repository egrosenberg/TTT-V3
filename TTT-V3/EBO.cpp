#include "EBO.h"

/**
 * Create Element Buffer Object
 * 
 * @param indices: 1d array containing order to draw indices in
 * @param size: size of indices
 */
EBO::EBO(std::vector <GLuint>* indices)
{
	// generate inex buffer
	glGenBuffers(1, &m_ID);
	// Bind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), indices->data(), GL_STATIC_DRAW);
}

// Binds the EBO
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

// Unbinds the EBO
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void EBO::Delete()
{
	glDeleteBuffers(1, &m_ID);
}

//Destructor
EBO::~EBO()
{
	Delete();
}