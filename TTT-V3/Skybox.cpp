#include "Skybox.h"

Skybox::Skybox(std::vector<std::string> *faceTextures, GLuint cubemapSlot)
{
    // Create VAO
    m_VAO= new VAO();
    m_VAO->Bind();
    // Create VBO and EBO
    m_VBO = new VBO(&SkyboxVertices);
    m_EBO = new EBO(&SkyboxIndices);
    // Link attributes to VAO
    m_VAO->LinkAttrib(m_VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    // Unbind VAO, VBO, and EBO to prevent further modification
    m_VAO->Unbind();
    m_VBO->Unbind();
    m_EBO->Unbind();

    // Create cubemap
    m_Cubemap = new CubeMap(faceTextures, cubemapSlot);
}

// Define uniforms for camera transformations
void Skybox::CamUniforms(Shader *shader, Camera *camera, float screenRatio)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    // compress view to mat3 and then back to mat4 so that the last two collumns are set to zero
    glm::vec3 camPos = camera->GetPos();
    view = glm::mat4(glm::mat3(glm::lookAt(camPos, camPos + camera->GetDirection(), camera->GetUp())));
    // set projection matrix
    projection = glm::perspective(glm::radians(45.0f), screenRatio, 0.1f, 100.0f);
    // export matrices to vertex shader
    glUniformMatrix4fv(glGetUniformLocation(shader->ID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Skybox::Draw(Shader *shader, Camera *camera, float screenRatio)
{
    // Disable face culling and change depth function so skybox renders behind everything properly
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    // activate shader and bind VAO / Texture
    shader->Activate();
    m_VAO->Bind();
    m_Cubemap->Bind();
    // Send camera uniforms to shader
    CamUniforms(shader, camera, screenRatio);
    // Draw triangles
    glDrawElements(GL_TRIANGLES, SKYBOX_VERT_N, GL_UNSIGNED_INT, 0);
    // unbind VAO and Cubemap
    m_VAO->Unbind();
    m_Cubemap->Unbind();
}

Skybox::~Skybox()
{
    // Delete all created objects
    delete m_VAO;
    delete m_VBO;
    delete m_EBO;

    delete m_Cubemap;
}
