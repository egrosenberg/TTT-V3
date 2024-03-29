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
private:
    GLuint m_ID;
    bool m_geomPresent;

    void CompileErrors(unsigned int shader, const char* type);
public:
    Shader(const char *vertexFile, const char *fragmentFile, const char *geometryFile = "none");
    virtual ~Shader();

    void Activate();
    void Delete();

    GLuint ID() { return m_ID; }
};

#endif