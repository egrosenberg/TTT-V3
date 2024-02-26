#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;

	// Create and bind VAO
	m_VAO = new VAO();
	m_VAO->Bind();

	// create VBO and EBO
	m_VBO = new VBO(&vertices);
	m_EBO = new EBO(&indices);

	// Link our VBO to our VAO
	m_VAO->LinkAttrib(m_VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_VAO->LinkAttrib(m_VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	m_VAO->LinkAttrib(m_VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	m_VAO->LinkAttrib(m_VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// unbind so we dont accidentally modify them
	m_VAO->Unbind();
	m_VBO->Unbind();
	m_EBO->Unbind();
}

void Mesh::Draw(Shader* shader, Camera* camera)
{
	// activate shader and bind vao
	shader->Activate();
	m_VAO->Bind();

	// track number of each type of texture
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
	// go through each texture and check texture type
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		std::string num;
		std::string type = m_textures[i].getType();
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		// send texture to shader
		m_textures[i].texUnit(shader, (type + num).c_str(), i);
		m_textures[i].Bind();
	}
	// send uniforms to shader
	glm::vec3 camPos = camera->GetPos();
	glUniform3f(glGetUniformLocation(shader->m_ID, "camPos"), camPos.x, camPos.y, camPos.z);
	camera->Matrix(shader, "camMatrix");

	// draw our mesh
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh()
{
	delete m_VAO;
	delete m_VBO;
	delete m_EBO;
}