#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"
#include "TTTcog.h"

#define CAM_SPEED 0.2f
#define CAM_SPRINT 1.0f
#define CAM_SENS 100.0f

class Camera : public TTTcog
{
private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;

    glm::mat4 m_cameraMatrix;

    int m_width;
    int m_height;

    float m_speed;
    float m_sensitivity;

    bool m_firstClick;

    std::string inline TestFn(void *v)
    {
        // check to verify pointer is valid
        if (v)
        {
            float *n = (float*)v;
            return std::to_string(*n);
        }
        return "invalid pointer";
    }


public:
    Camera(int width, int height, glm::vec3 position);

    void UpdateMatrix(float FOVdeg, float nearDist, float farDist);
    void Matrix(Shader* shader, const char* uniform);
    void Inputs(GLFWwindow* window);

    // getters and setters for up
    void SetUp(glm::vec3 up) { m_up = up; }
    glm::vec3 GetUp() { return m_up; }
    
    glm::vec3 GetPos() { return m_position; }
    glm::vec3 GetDirection() { return m_direction; }

    virtual ~Camera();
};

#endif