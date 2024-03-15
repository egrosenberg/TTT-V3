#include "main.h"

// vertices for skybox cube
float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

// indices for skybox cube
unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
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
	GLFWwindow* winMain = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, NAME, NULL, NULL);
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
	Shader *shaderProgram = new Shader("Shaders/default.vert", "Shaders/default.frag", "Shaders/default.geom");
	Shader *wireProgram = new Shader("Shaders/default.vert", "Shaders/wireframe.frag", "Shaders/wireframe.geom");
	// set up shader for framebuffer
	Shader *frameProgram = new Shader("Shaders/framebuffer.vert", "Shaders/framebuffer.frag");
	// set up shader for skybox
	Shader *skyboxProgram = new Shader("Shaders/skybox.vert", "Shaders/skybox.frag");

	// def light color
	glm::vec4 lightColor = glm::vec4(1.00f, 0.77f, 0.77f, 1.0f);
	// define position and model for light
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	// Activate model shader and set uniforms
	shaderProgram->Activate();
	glUniform4f(glGetUniformLocation(shaderProgram->ID(), "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram->ID(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	// Activate framebuffer shader and set uniform
	frameProgram->Activate();
	glUniform1i(glGetUniformLocation(frameProgram->ID(), "screenTexture"), 0);
	// Activate skybox shader
	skyboxProgram->Activate();
	glUniform1i(glGetUniformLocation(skyboxProgram->ID(), "skybox"), 0);


	// enable depth testing buffer
	glEnable(GL_DEPTH_TEST);
	// set depth function
	glDepthFunc(GL_LESS);

	// enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	// create camera object
	Camera* mainCamera = new Camera(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	// create model object
	Model* model = new Model("models/cat/scene.gltf");


	// transform the model
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 scale = glm::vec3(0.05f);
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, translation);
	transform = glm::rotate(transform, glm::radians(-90.0f), rotation);
	transform = glm::scale(transform, scale);

	// set transform of model
	model->SetTransform(transform);

	// set up variables for FPS tracking
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff = 0.0;
	unsigned int frameCounter = 0;

	// set up post proccessing display
	Display* display = new Display();

	// create an array of strings to hold cubemap faces
	std::vector<std::string> skyboxFaces =
	{
		"Textures/Skybox/px.png",
		"Textures/Skybox/nx.png",
		"Textures/Skybox/py.png",
		"Textures/Skybox/ny.png",
		"Textures/Skybox/pz.png",
		"Textures/Skybox/nz.png",
	};

	// Create skybox
	Skybox *skybox = new Skybox(&skyboxFaces, 0);

	// boolean to track if we should draw wireframe
	bool wireframe = false;
	bool q_pressed = false;

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

		// Clean the back depth and color buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// make sure depth testing is enabled
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		// enable face culling
		glEnable(GL_CULL_FACE);

		// Camera control
		mainCamera->Inputs(winMain);
		// update camera matrix
		mainCamera->UpdateMatrix(FOV, 0.1f, 100.0f);

		// draw model
		model->Draw(shaderProgram, mainCamera);

		if (glfwGetKey(winMain, GLFW_KEY_Q) == GLFW_PRESS)
		{
			if (q_pressed == false)
			{
				wireframe = wireframe ? false : true;
				q_pressed = true;
			}
		}
		else
		{
			q_pressed = false;
		}
		if (wireframe)
		{
			model->Draw(wireProgram, mainCamera);
		}

		// draw skybox
		skybox->Draw(skyboxProgram, mainCamera, (float)WIN_WIDTH / WIN_HEIGHT);

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
	delete skybox;

	// close window
	glfwDestroyWindow(winMain);
	// terminate glfw
	glfwTerminate();
	return 0;
}
