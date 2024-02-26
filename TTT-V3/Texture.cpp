#include "Texture.h"

Texture::Texture(const char* fname, const char *texType, GLuint slot, GLenum format, GLenum pixelType)
{
	m_type = texType;

	// Load image
	stbi_set_flip_vertically_on_load(true);
	int widthImg, heightImg, numColCh;
	unsigned char* bytes = stbi_load(fname, &widthImg, &heightImg, &numColCh, 0);

	// Create and generate our texture
	glGenTextures(1, &m_ID);
	// activate texture unit
	glActiveTexture(GL_TEXTURE0 + slot);
	m_unit = slot;
	// bind texture
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// set scaling mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// set repeat mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	// delete img data and unbind texture
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader* shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader->m_ID, uniform);
	shader->Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + m_unit);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &m_ID);
}

Texture::~Texture()
{
	Delete();
}