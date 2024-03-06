#ifndef DISPLAY_H
#define DISPLAY_H

#include "main.h"

// verts for post processing rect
const float SCREEN_VERTS[] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

class Display
{
private:
	GLuint m_ID;
	GLuint m_Tex;
	GLuint m_VAO;
	GLuint m_RBO;

public:
	Display();

	void Bind(GLuint internalW, GLuint internalH);
	void Unbind();
	void Draw(Shader *shader, GLFWwindow *window);

	virtual ~Display();
};

#endif