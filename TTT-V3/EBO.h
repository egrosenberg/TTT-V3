#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>
#include <vector>

class EBO
{
public:
    GLuint m_ID;
    EBO(std::vector <GLuint> *indices);

    void Bind();
    void Unbind();
    void Delete();

    virtual ~EBO();
};

#endif