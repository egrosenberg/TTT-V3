#include "main.h"

// create some vertices to draw 3 triangles
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS     /  coordinates //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f   // Lower right corner
};

// mark rendering order indices for vertices
GLuint indices[] =

{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};

int main()
{
	// initialize glfw
	glfwInit();

	// Mark OpenGL version in glfw
	// This project is using version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW to use the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create our window and ensure it opened properly
	GLFWwindow *winMain = glfwCreateWindow(800, 800, NAME, NULL, NULL);
	if (!winMain)
	{
		std::cerr << "Failed to initalize window, exiting." << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce window to current context
	glfwMakeContextCurrent(winMain);

	// init glad
	gladLoadGL();

	// Define the viewport of the OpenGL window
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	// set up shader program
	Shader* shaderProgram = new Shader("default.vert", "default.frag");

	// Create and bind VAO
	VAO *VAO1 = new VAO();
	VAO1->Bind();

	// create VBO and EBO
	VBO *VBO1 = new VBO(vertices, sizeof(vertices));
	EBO *EBO1 = new EBO(indices, sizeof(indices));

	// Link our VBO to our VAO
	VAO1->LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1->LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1->LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1->Unbind();
	VBO1->Unbind();
	EBO1->Unbind();

	// Get scale uniform from shaderprogram
	GLuint uniID = glGetUniformLocation(shaderProgram->m_ID, "scale");

	// Create and generate our texture
	Texture* cat2 = new Texture("Textures/cat2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	
	// set up texture unit
	cat2->texUnit(shaderProgram, "tex0", 0);

	// Main loop
	while (!glfwWindowShouldClose(winMain))
	{
		// Specify the background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which shader program to use
		shaderProgram->Activate();
		// set uniform scale to
		glUniform1f(uniID, 0.5f);
		// bind cat texture
		cat2->Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1->Bind();
		// Draw primitives
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap the back and front buffers
		glfwSwapBuffers(winMain);
		// Trigger all GLFW events
		glfwPollEvents();
	}

	// clean up objects used for shaders and drawing
	delete VAO1;
	delete VBO1;
	delete EBO1;

	delete cat2;

	delete shaderProgram;


	// close window
	glfwDestroyWindow(winMain);
	// terminate glfw
	glfwTerminate();
	return 0;
}