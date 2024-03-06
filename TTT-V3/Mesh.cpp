#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices, std::vector <Texture*> &textures)
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

void Mesh::Draw(
	Shader* shader,
	Camera* camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
	)
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
		std::string type = m_textures[i]->getType();
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		// send texture to shader
		m_textures[i]->texUnit(shader, (type + num).c_str(), i);
		m_textures[i]->Bind();
	}
	// send uniforms to shader
	glm::vec3 camPos = camera->GetPos();
	glUniform3f(glGetUniformLocation(shader->m_ID, "camPos"), camPos.x, camPos.y, camPos.z);
	camera->Matrix(shader, "camMatrix");

	// calculate transformation matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// send transformation matrices as uniforms
	glUniformMatrix4fv(glGetUniformLocation(shader->m_ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader->m_ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader->m_ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader->m_ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	// draw our mesh
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

// deletes texture at index and replaces it
void Mesh::ReplaceTex(unsigned int index, Texture *tex)
{
	// error checking
	if (index >= m_textures.size())
	{
		std::cout << "ERROR: invalid texture index" << std::endl;
		return;
	}
	delete m_textures[index];
	m_textures[index] = tex;
}

Mesh::~Mesh()
{
	delete m_VAO;
	delete m_VBO;
	delete m_EBO;
}