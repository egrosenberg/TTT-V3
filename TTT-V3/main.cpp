#include "main.h"

int main()
{
	// create some vertices to draw 3 triangles
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6 , 0.0f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
	};

	// mark rendering order indices for vertices
	GLuint indices[] =
	{
		0, 3, 5, // Lower Left Triangle
		3, 2, 4, // Lower Right Triangle
		5, 4, 1  // Uper Triangle
	};


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
	VAO1->LinkVBO(VBO1, 0);
	VAO1->Unbind();
	VBO1->Unbind();
	EBO1->Unbind();

	
	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// swap the back buffer with the front buffer
	glfwSwapBuffers(winMain);

	// Main loop
	while (!glfwWindowShouldClose(winMain))
	{
		// Specify the background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which shader program to use
		shaderProgram->Activate();
		// Bind the VAO so OpenGL knows to use it
		VAO1->Bind();
		// Draw primitives
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap the back and front buffers
		glfwSwapBuffers(winMain);
		// Trigger all GLFW events
		glfwPollEvents();
	}

	// clean up objects used for shaders and drawing
	delete VAO1;
	delete VBO1;
	delete EBO1;
	delete shaderProgram;


	// close window
	glfwDestroyWindow(winMain);
	// terminate glfw
	glfwTerminate();
	return 0;
}