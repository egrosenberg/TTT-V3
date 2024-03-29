#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
    GLuint m_ID;
    VAO();

    void LinkAttrib(VBO *vbo, GLuint layout, GLuint componentN, GLuint type, GLsizeiptr stride, void *offset);
    void Bind();
    void Unbind();
    void Delete();

    virtual ~VAO();
};

#endif