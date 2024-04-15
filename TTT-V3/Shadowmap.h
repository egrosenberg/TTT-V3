#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "TTT.h"
#include "FBO.h"
#include "Model.h"

class Shadowmap
{
private:
    FBO *m_FBO;
    glm::vec3 m_lightPos;
    int m_type;
    bool m_bound;
    unsigned int m_width;
    unsigned int m_height;
    float m_farplane;

public:
    Shadowmap(int type, unsigned int width, unsigned int height, float farPlane, glm::vec3 lightPosition, Shader *shader);

    void SetUniforms(Shader *shader, const char *uniform = "lightProj");
    void Bind();
    void Unbind();
    void BindTex();

    void SetLPos(glm::vec3 pos) { m_lightPos = pos; }

    virtual ~Shadowmap();
};

#endif