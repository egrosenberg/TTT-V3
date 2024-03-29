#include "VBO.h"

VBO::VBO(std::vector <Vertex> *vertices)
{
    // generate our buffer for one object
    glGenBuffers(1, &m_ID);

    // bind our buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    // store our vertices in the VBO
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), vertices->data(), GL_STATIC_DRAW);
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
    glDeleteBuffers(1, &m_ID);
}

VBO::~VBO()
{
    Delete();
}