#ifndef GBUFFER_H
#define GBUFFER_H

#include "RBO.h"
#include "TTT.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class Gbuffer
{
private:
    GLuint m_width;
    GLuint m_height;
    GLuint m_FBO;
    std::vector<GLuint> *m_textures;

public:
    Gbuffer(unsigned int width, unsigned int height);

    void BindFBO(GLenum target = GL_FRAMEBUFFER);
    unsigned int BindTextures(Shader* shader);

    unsigned int size() { return m_textures->size(); }

    virtual ~Gbuffer();
};

#endif