#include "Gbuffer.h"

#define G_POSITION      0
#define G_NORMAL        1
#define G_ALBEDO_SPEC   2
#define N_GTEXTURES     3

Gbuffer::Gbuffer(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;

    // generate fbo for g-buffer
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // gen textures and bind them to gbuffer
    m_textures = new std::vector<GLuint>();

    // position buffer
    m_textures->push_back(0);
    glGenTextures(1, &(*m_textures)[G_POSITION]);
    glBindTexture(GL_TEXTURE_2D, (*m_textures)[G_POSITION]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + G_POSITION, GL_TEXTURE_2D, (*m_textures)[G_POSITION], 0);
    // normal buffer
    m_textures->push_back(0);
    glGenTextures(1, &(*m_textures)[G_NORMAL]);
    glBindTexture(GL_TEXTURE_2D, (*m_textures)[G_NORMAL]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + G_NORMAL, GL_TEXTURE_2D, (*m_textures)[G_NORMAL], 0);
    // color + specular buffer
    m_textures->push_back(0);
    glGenTextures(1, &(*m_textures)[G_ALBEDO_SPEC]);
    glBindTexture(GL_TEXTURE_2D, (*m_textures)[G_ALBEDO_SPEC]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + G_ALBEDO_SPEC, GL_TEXTURE_2D, (*m_textures)[G_ALBEDO_SPEC], 0);
    // set up color attachments with opengl
    GLuint *attachments = new GLuint[m_textures->size()];
    for (unsigned int i = 0; i < G_BUFFER_DEPTH; ++i)
    {
        attachments[i] = (GLuint)(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(G_BUFFER_DEPTH, attachments);
    // generate a renderbuffer
    RBO rbo(m_width, m_height, 0);
    // check if fbo is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Error in creating g-buffer" << std::endl;
    }
    delete[] attachments;
}

void Gbuffer::BindFBO(GLenum target)
{
    glBindFramebuffer(target, m_FBO);
}

unsigned int Gbuffer::BindTextures(Shader *shader)
{
    glUniform1i(glGetUniformLocation(shader->ID(), "gPosition"), G_POSITION);
    glActiveTexture(GL_TEXTURE0 + G_POSITION);
    glBindTexture(GL_TEXTURE_2D, (*m_textures)[G_POSITION]);
    glUniform1i(glGetUniformLocation(shader->ID(), "gNormal"), G_NORMAL);
    glActiveTexture(GL_TEXTURE0 + G_NORMAL);
    glBindTexture(GL_TEXTURE_2D, (*m_textures)[G_NORMAL]);
    glUniform1i(glGetUniformLocation(shader->ID(), "gAlbedoSpec"), G_ALBEDO_SPEC);
    glActiveTexture(GL_TEXTURE0 + G_ALBEDO_SPEC);
    glBindTexture(GL_TEXTURE_2D, (*m_textures)[G_ALBEDO_SPEC]);
    return m_textures->size();
}

Gbuffer::~Gbuffer()
{
    delete m_textures;
}