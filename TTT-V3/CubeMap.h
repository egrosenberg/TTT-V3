#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <vector>
#include <string>
#include <iostream>

class CubeMap
{
private:
	GLuint m_ID;
	GLuint m_unit;

public:
	CubeMap(std::vector<std::string> *paths, GLuint slot);

	void Bind();
	void Unbind();

	virtual ~CubeMap();
};

#endif