#ifndef SKYBOX_H
#define SKYBOX_H

#include "CubeMap.h"
#include "VAO.h"
#include "EBO.h"
#include "Shader.h"
#include "Camera.h"

#define SKYBOX_VERT_N 36

class Skybox
{
private:
	// vertices for drawing skyboxes
	std::vector<Vertex> SkyboxVertices =
	{
		{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)}
	};
	// indices for draw order of skyboxes
	std::vector<unsigned int> SkyboxIndices =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	CubeMap *m_Cubemap;
	VAO *m_VAO;
	VBO *m_VBO;
	EBO *m_EBO;

	void CamUniforms(Shader *shader, Camera *camera, float screenRatio);
public:
	Skybox(std::vector<std::string> *faceTextures, GLuint cubemapSlot);

	void Draw(Shader *shader, Camera *camera, float winRatio);

	virtual ~Skybox();
};

#endif