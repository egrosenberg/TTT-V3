#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <cerrno>

class Shader
{
public:
	Shader(const char* vertexFile, const char* fragmentFile);
	virtual ~Shader();

	void Activate();
	void Delete();

	GLuint ID() { return m_ID; }
private:
	GLuint m_ID;

	void CompileErrors(unsigned int shader, const char* type);
};

#endif