#include "Texture.h"

Texture::Texture(const char* fname, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	m_type = texType;

	// Load image
	stbi_set_flip_vertically_on_load(true);
	int widthImg, heightImg, numColCh;
	unsigned char* bytes = stbi_load(fname, &widthImg, &heightImg, &numColCh, 0);

	// Create and generate our texture
	glGenTextures(1, &m_ID);
	// activate texture unit
	glActiveTexture(slot);
	// bind texture
	glBindTexture(texType, m_ID);

	// set scaling mode
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// set repeat mode
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate texture
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	// delete img data and unbind texture
	stbi_image_free(bytes);
	glBindTexture(m_type, 0);
}

void Texture::texUnit(Shader* shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader->m_ID, uniform);
	shader->Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glBindTexture(m_type, m_ID);
}

void Texture::Unbind()
{
	glBindTexture(m_type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &m_ID);
}

Texture::~Texture()
{
	Delete();
}