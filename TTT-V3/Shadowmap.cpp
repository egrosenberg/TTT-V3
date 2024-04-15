#include "Shadowmap.h"

#define CUBE_FACES 6
#define SHADOW_W 50.0f

/**
 * Constructor
 * 
 * @param type: type of point light (POINT_LIGHT, SPOT_LIGHT, or DIREC_LIGHT)
 * @param width: width of shadowmap view
 * @param height: height of shadowmap view
 * @param farPlane: far plane for shadowmap
 * @param lightSource: TTTlight object for the source of the shadowmap
 * @param shader: Shader to use for rendering to the shadowmap FBO
 */
Shadowmap::Shadowmap(int type, unsigned int width, unsigned int height, float farPlane, glm::vec3 lightPosition, Shader *shader)
{
    if (type < POINT_LIGHT || type > DIREC_LIGHT)
    {
        type = POINT_LIGHT;
    }
    m_type = type;
    m_bound = false;
    m_width = width;
    m_height = height;
    m_farplane = farPlane;
    m_lightPos = lightPosition;

    if (m_type == POINT_LIGHT)
    {
        m_FBO = new FBO(m_width, m_height, TTTenum::TTT_DEPTH_FRAMEBUFFER, TTTenum::TTT_TEXTURE_CUBEMAP);
    }
    else
    {
        m_FBO = new FBO(m_width, m_width, TTTenum::TTT_DEPTH_FRAMEBUFFER, TTTenum::TTT_TEXTURE_2D);
    }

    SetUniforms(shader);
}

/**
 * Sets the matrix uniforms in the shadowmap's shader program based on a light source
 * 
 * @param shader: shader to bind uniforms to
 */
void Shadowmap::SetUniforms(Shader *shader, const char* uniform)
{
    if (m_type == POINT_LIGHT)
    {
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, m_farplane);
        glm::mat4 shadowTransforms[] =
        {
            shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
            shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
            shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)),
            shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)),
            shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)),
            shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0))
        };
        // activate shaderprogram and pass uniforms
        shader->Activate();
        glUniformMatrix4fv(glGetUniformLocation(shader->ID(), "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID(), "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID(), "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID(), "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID(), "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID(), "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));
        glUniform3f(glGetUniformLocation(shader->ID(), "lightPos"), m_lightPos.x, m_lightPos.y, m_lightPos.z);
        glUniform1f(glGetUniformLocation(shader->ID(), "farPlane"), m_farplane);
    }
    else
    {
        glm::mat4 shadowProj = glm::mat4(1.0f);
        glm::mat4 lightView = glm::mat4(1.0f);
        if (m_type == SPOT_LIGHT)
        {
            shadowProj = glm::perspective(glm::radians(130.0f), 1.0f, 0.1f, m_farplane);
            lightView = glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else if (m_type == DIREC_LIGHT)
        {
            shadowProj = glm::ortho(-SHADOW_W, SHADOW_W, -SHADOW_W, SHADOW_W, 0.1f, m_farplane);
            lightView = glm::lookAt(5.0f * m_lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        glm::mat4 lightProj = shadowProj * lightView;
        shader->Activate();
        glUniformMatrix4fv(glGetUniformLocation(shader->ID(), uniform), 1, GL_FALSE, glm::value_ptr(lightProj));
    }
}

/**
 * Binds shadow cube map / prepares for rendering
 */
void Shadowmap::Bind()
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, m_width, m_height);
    m_FBO->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
}

/**
 * Unbinds the FBO
 */
void Shadowmap::Unbind()
{
    m_FBO->Unbind();
}

/**
 * Binds the depth texture from the shadow map fbo
 */
void Shadowmap::BindTex()
{
    m_FBO->BindTex();
}

Shadowmap::~Shadowmap()
{
    delete m_FBO;
}
