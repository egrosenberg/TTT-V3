#include "Pilotable.h"

Pilotable::Pilotable(Scene *scene, Camera *camera, GLFWwindow *window)
: TTTobject(scene)
{
    m_camera = camera;
    m_window = window;
}

void Pilotable::FixedUpdate()
{
    TTTobject::FixedUpdate();

    // TODO: Fix magic numbers
    // process movements
    // forward
    glm::vec3 direction = glm::vec3(0.0f);
    direction.x = glm::sin(p_rotation.y);
    direction.z = glm::cos(p_rotation.y);
    direction = glm::normalize(direction);
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        p_velocity = 30.0f * direction;
    }
    else if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        p_velocity = -30.0f * direction;
    }
    else
    {
        p_velocity = glm::vec3(0.0f);
    }
    // rotate
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        p_angularVelocity = glm::vec3(0.0f, glm::radians(-180.0f), 0.0f);
    }
    else if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        p_angularVelocity = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);
    }
    else
    {
        p_angularVelocity = glm::vec3(0.0f);
    }
    // up / down
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        p_velocity.y = 30.0f;
    }
    else if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        p_velocity.y = -30.0f;
    }
    else
    {
        p_velocity.y = 0.0f;
    }

    glm::vec3 targetCamPos = p_position + glm::vec3(0.0f, 5.0f, 0.0f) - 10.0f * direction;
    glm::vec3 targetCamDir = glm::normalize(p_position - m_camera->GetPos());

    glm::vec3 camPosDiff = targetCamPos - m_camera->GetPos();
    glm::vec3 camDirDiff = targetCamDir - m_camera->GetDirection();

    glm::vec3 newCamPos = m_camera->GetPos() + (camPosDiff * 0.05f);
    glm::vec3 newCamDir = m_camera->GetDirection() + (camDirDiff * 0.1f);

    m_camera->SetPos(newCamPos);
    m_camera->SetDirection(newCamDir);
}

Pilotable::~Pilotable()
{

}