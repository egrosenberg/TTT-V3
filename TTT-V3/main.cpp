#include "main.h"


// verts for post processing rect
float rectangleVertices[] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

std::string readFile(const char* filename)
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

	// set up shader program for main object
	Shader *shaderProgram = new Shader("default.vert", "default.frag");
	// set up shader for framebuffer
	Shader *frameProgram = new Shader("framebuffer.vert", "framebuffer.frag");

	// def light color
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// define position and model for light
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	// Activate model shader and set uniforms
	shaderProgram->Activate();
	glUniform4f(glGetUniformLocation(shaderProgram->m_ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram->m_ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	// Activate framebuffer shader and set uniform
	frameProgram->Activate();
	glUniform1i(glGetUniformLocation(frameProgram->m_ID, "screenTexture"), 0);



	// enable depth testing buffer
	glEnable(GL_DEPTH_TEST);
	// set depth function
	glDepthFunc(GL_LESS);

	// enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	// create camera object
	Camera* mainCamera = new Camera(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	// create model object
	Model *model = new Model("models/cat/scene.gltf");


	// transform the model
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 scale = glm::vec3(0.05f);
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, translation);
	transform = glm::rotate(transform, glm::radians(90.0f), rotation);
	transform = glm::scale(transform, scale);

	// set transform of model
	model->SetTransform(transform);

	// set up variables for FPS tracking
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff = 0.0;
	unsigned int frameCounter = 0;

	Display* display = new Display();

	// Main loop
	while (!glfwWindowShouldClose(winMain))
	{
		// calculate fps
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		++frameCounter;
		if (timeDiff >= 1.0 / 30.0)
		{
			std::string fps = std::to_string((1.0 / timeDiff) * frameCounter);
			std::string ms = std::to_string((timeDiff / frameCounter) * 1000);
			std::string newTitle = NAME + (" - " + fps + "fps / " + ms + "ms");
			glfwSetWindowTitle(winMain, newTitle.c_str());
			prevTime = crntTime;
			frameCounter = 0;
		}

		// bind display buffer before drawing
		display->Bind(WIN_WIDTH, WIN_HEIGHT);

		// Specify the background color
		glClearColor(0.85f, 0.85f, 0.90f, 1.0f);
		// Clean the back depth and color buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// make sure depth testing is enabled
		glEnable(GL_DEPTH_TEST);
		// enable face culling
		glEnable(GL_CULL_FACE);

		// Camera control
		mainCamera->Inputs(winMain);
		// update camera matrix
		mainCamera->UpdateMatrix(FOV, 0.1f, 100.0f);

		// draw model
		model->Draw(shaderProgram, mainCamera);

		// unbind display buffer
		display->Unbind();

		// draw display buffer
		display->Draw(frameProgram, winMain);

		// Swap the back and front buffers
		glfwSwapBuffers(winMain);
		// Trigger all GLFW events
		glfwPollEvents();
	}

	// clean up objects used for shaders and drawing
	delete model;
	delete shaderProgram;
	delete frameProgram;
	delete display;

	// close window
	glfwDestroyWindow(winMain);
	// terminate glfw
	glfwTerminate();
	return 0;
}
