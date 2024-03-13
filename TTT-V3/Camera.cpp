#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	m_firstClick = false;

	// intialize directions
	m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	// initializse camera speeds
	m_speed = CAM_SPEED;
	m_sensitivity = CAM_SENS;

	// initialize dimmensions
	m_width = width;
	m_height = height;
	
	// initalize position
	m_position = position;

	m_cameraMatrix = glm::mat4(1.0f);
}

// calculares view and projection matrices based on camera
void Camera::UpdateMatrix(float FOVdeg, float nearDist, float farDist)
{
	// initialize matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// make view look up
	view = glm::lookAt(m_position, m_position + m_direction, m_up);
	
	// set perspective matrix based on params
	projection = glm::perspective(FOVdeg, (float)(m_width / m_height), nearDist, farDist);

	m_cameraMatrix = projection * view;
}

void Camera::Matrix(Shader* shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader->ID(), uniform), 1, GL_FALSE, glm::value_ptr(m_cameraMatrix));
}


// process movement inputs for camera
void Camera::Inputs(GLFWwindow* window)
{
	// forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_position += m_speed * m_direction;
	}
	// left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_position += m_speed * -glm::normalize(glm::cross(m_direction, m_up));
	}
	// back
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_position += m_speed * -m_direction;
	}
	// right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_position += m_speed * glm::normalize(glm::cross(m_direction, m_up));
	}
	// up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_position += m_speed * m_up;
	}
	// down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_position += m_speed * -m_up;
	}
	// sprint_start
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_speed = CAM_SPRINT;
	}
	// sprint_end
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
	{
		m_speed = CAM_SPEED;
	}

	// hide / show mouse when clicking
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// if first click, center mouse
		if (m_firstClick)
		{
			glfwSetCursorPos(window, (m_width / 2), (m_height / 2));
			m_firstClick = false;
		}

		// get mouse position
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// calculate rotation and normalize cursor so that it begins in normall of screen
		float rotX = m_sensitivity * (float)(mouseY - (m_height / 2)) / m_height;
		float rotY = m_sensitivity * (float)(mouseX - (m_width / 2)) / m_width;

		// calculate vertical change in orientation
		glm::vec3 newOrientation = glm::rotate(m_direction, glm::radians(-rotX), glm::normalize(glm::cross(m_direction, m_up)));

		// decide if new orientation is valid (make sure we dont flip over ourselves on x rotation
		if (abs(glm::angle(newOrientation, m_up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			m_direction = newOrientation;
		}

		// rotate along y, we dont want to limit this
		m_direction = glm::rotate(m_direction, glm::radians(-rotY), m_up);

		// set mouse pos to center of screen
		glfwSetCursorPos(window, (m_width * 0.5f), (m_height * 0.5f));
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// reset first click
		m_firstClick = true;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

Camera::~Camera()
{

}
