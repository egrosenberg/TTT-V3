#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
    glm::vec3 position; // vertex postion
    glm::vec3 normal;	// vertex normal
    glm::vec3 color;	// vertex color (rgb)
    glm::vec2 texUV;	// vertetex texture coordinate
};

class VBO
{
public:
    GLuint m_ID;
    VBO(std::vector <Vertex> *vertices);

    void Bind();
    void Unbind();
    void Delete();

    virtual ~VBO();
};

#endif