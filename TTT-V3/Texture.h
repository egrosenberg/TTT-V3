#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Shader.h"

class Texture
{
private:
	GLuint m_ID;
	GLuint m_type;
	GLuint m_unit;

public:
	Texture(const char *fname, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	void texUnit(Shader *shader, const char *uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

	GLuint getID() { return m_ID; }
	GLuint getType() { return m_type; }

	virtual ~Texture();
};

#endif