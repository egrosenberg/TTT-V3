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
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// set up fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// create shader program
	m_ID = glCreateProgram();

	// attach shaders to our shader program
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);

	// link our program
	glLinkProgram(m_ID);

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