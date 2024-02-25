#include "main.h"

// create some vertices to draw 3 triangles
GLfloat vertices[] =
{ //     vertex coords   /        color coords    /    tex coords  /        normals       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

// mark rendering order indices for vertices
GLuint indices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

// create vertices for light source
GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

// mark rendering order for light source
GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
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
	GLFWwindow *winMain = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, NAME, NULL, NULL);
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
	VAO1->LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1->LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1->LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1->LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// unbind so we dont accidentally modify them
	VAO1->Unbind();
	VBO1->Unbind();
	EBO1->Unbind();

	// create shader program for light
	Shader *lightShader = new Shader("light.vert", "light.frag");
	// create and bind light vao
	VAO *lightVAO = new VAO();
	lightVAO->Bind();

	// create light vbo and ebo
	VBO *lightVBO = new VBO(lightVertices, sizeof(lightVertices));
	EBO* lightEBO = new EBO(lightIndices, sizeof(lightIndices));
	// link light vbo to vao
	lightVAO->LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	// unbind light vao etc. so we dont modify later
	lightVAO->Unbind();
	lightVBO->Unbind();
	lightEBO->Unbind();


	// def light color
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// define position and model for light
	glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	// define position for pyramid model
	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	// Activate light shader and set uniforms
	lightShader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader->m_ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader->m_ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	// Activate pyramid shader and set uniforms
	shaderProgram->Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->m_ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram->m_ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram->m_ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Create and generate our texture
	Texture* cat2 = new Texture("Textures/cat2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	// set up texture unit
	cat2->texUnit(shaderProgram, "tex0", 0);
	
	// enable depth testing buffer
	glEnable(GL_DEPTH_TEST);

	// create camera object
	Camera* mainCamera = new Camera(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main loop
	while (!glfwWindowShouldClose(winMain))
	{
		// Specify the background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back depth and color buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Camera control
		mainCamera->Inputs(winMain);
		// update camera matrix
		mainCamera->UpdateMatrix(FOV, 0.1f, 100.0f);
		// Tell OpenGL which shader program to use
		shaderProgram->Activate();
		// Send camera location data for specular lighting
		glm::vec3 cameraPos = mainCamera->GetPos();;
		glUniform3f(glGetUniformLocation(shaderProgram->m_ID, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
		mainCamera->Matrix(shaderProgram, "camMatrix");
		// bind cat texture
		cat2->Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1->Bind();
		// Draw primitives
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		// draw light source
		lightShader->Activate();
		mainCamera->Matrix(lightShader, "camMatrix");
		lightVAO->Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

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


	delete lightVAO;
	delete lightVBO;
	delete lightEBO;

	delete shaderProgram;

	delete lightShader;

	// close window
	glfwDestroyWindow(winMain);
	// terminate glfw
	glfwTerminate();
	return 0;
}