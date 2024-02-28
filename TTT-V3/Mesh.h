#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
private:
	std::vector <Vertex> m_vertices;
	std::vector <GLuint> m_indices;
	std::vector <Texture*> m_textures;

	VAO *m_VAO;
	VBO *m_VBO;
	EBO *m_EBO;

public:
	Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices, std::vector <Texture*> &textures);

	void Draw(
		Shader* shader,
		Camera* camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);

	virtual ~Mesh();
};

#endif
