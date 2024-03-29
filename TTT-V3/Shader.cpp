#include "Shader.h"
#include "main.h"

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
{
    bool m_geomPresent = (strcmp(geometryFile, "none") != 0);
    // read vertex and fragment shader code from files
    std::string vertexCode = readFile(vertexFile);
    std::string fragmentCode = readFile(fragmentFile);
    std::string geometryCode = m_geomPresent ? readFile(geometryFile) : "";
    // convert shader code to c-strings
    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();
    const char* geometrySource = m_geomPresent ? geometryCode.c_str() :"";

    // set up vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // attach shader source code to the vertex shader object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // compile the vertex shader machine code
    glCompileShader(vertexShader);
    // compile vertex shader errors
    CompileErrors(vertexShader, "VERTEX");

    // set up fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // attach shader source code to fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // compile the fragment shader machine code
    glCompileShader(fragmentShader);
    // compile fragment shader errors
    CompileErrors(fragmentShader, "FRAGMENT");

    // compile geometry shader
    GLuint geometryShader = 0;
    if (m_geomPresent)
    {
        // set up geometry shader
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        // attach shader source code to geometry shader object
        glShaderSource(geometryShader, 1, &geometrySource, NULL);
        // compile the geometry shader machine code
        glCompileShader(geometryShader);
        // compile geometry shader errors
        CompileErrors(geometryShader, "GEOMETRY");
    }

    // create shader program
    m_ID = glCreateProgram();

    // attach shaders to our shader program
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    if (m_geomPresent) glAttachShader(m_ID, geometryShader);


    // link our program
    glLinkProgram(m_ID);
    // compile shader program errors
    CompileErrors(m_ID, "PROGRAM");

    // clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
    glUseProgram(m_ID);
}

void Shader::Delete()
{
    glDeleteProgram(m_ID);
}

Shader::~Shader()
{
    Delete();
}

void Shader::CompileErrors(unsigned int shader, const char* type)
{
    GLint hasCompiled;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;
        }
    }
}
