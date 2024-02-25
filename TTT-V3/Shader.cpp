#include "Shader.h"

std::string Shader::readFile(const char* filename)
{
	// create input stream
	std::ifstream in(filename, std::ios::binary);
	// verify file is open
	if (in)
	{
		// create buffer to store file
		std::string contents;
		// resize string to size of input file
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		// move back to begin of file
		in.seekg(0, std::ios::beg);
		// read file to contents
		in.read(&contents[0], contents.size());
		// close file
		in.close();
		// return file data
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// read vertex and fragment shader code from files
	std::string vertexCode = readFile(vertexFile);
	std::string fragmentCode = readFile(fragmentFile);

	// convert shader code to c-strings
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

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

	// create shader program
	m_ID = glCreateProgram();

	// attach shaders to our shader program
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);

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