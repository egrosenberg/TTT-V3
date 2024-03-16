#include "Display.h"

// scales viewport of window to fill current window size
void scaleViewport(GLFWwindow *window)
{
	// get current window resolution
	int cWidth, cHeight;
	glfwGetWindowSize(window, &cWidth, &cHeight);
	// find correct scaled version of resolution
	int offsetX = 0;
	int offsetY = 0;
	if (cHeight * WIN_RATIO > cWidth)
	{
		// window is too tall
		int prevH = cHeight;
		cHeight = cWidth / WIN_RATIO;
		offsetY = (prevH - cHeight) / 2;
	}
	else
	{
		// window is too wide
		int prevW = cWidth;
		cWidth = cHeight * WIN_RATIO;
		offsetX = (prevW - cWidth) / 2;
	}
	// set viewport to window resolution
	glViewport(offsetX, offsetY, cWidth, cHeight);
}

// default constructor
Display::Display(unsigned int samples)
{
	// create rectangle for post proccessing
	GLuint VBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SCREEN_VERTS), &SCREEN_VERTS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// initalize msaa buffer
	m_multisampleFBO = new FBO(WIN_WIDTH, WIN_HEIGHT, samples);

	// intialize render buffer
	m_RBO = new RBO(WIN_WIDTH, WIN_HEIGHT, samples);

	FBO::CheckStatus("MSAA");

	// initalize normal framebuffer
	m_FBO = new FBO(WIN_WIDTH, WIN_HEIGHT, 0);

	FBO::CheckStatus("POST-PROCESSING");
}

void Display::Bind(GLuint internalW, GLuint internalH)
{
	// set viewport to internal resolution
	glViewport(0, 0, internalW, internalH);
	// bind frame buffer
	m_multisampleFBO->Bind();
}

void Display::Unbind()
{
	m_multisampleFBO->Unbind();
}

// Draws the contents of the fbo to the screen, scaled to current window size
void Display::Draw(Shader *shader, GLFWwindow *window)
{
	// bind MSAA buffer as readonly & pp as writeonly
	m_multisampleFBO->Bind(GL_READ_FRAMEBUFFER);
	m_FBO->Bind(GL_DRAW_FRAMEBUFFER);
	// blit FBOs
	glBlitFramebuffer(0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// rebind default framebuffer
	m_FBO->Unbind();

	// scale window
	scaleViewport(window);
	// activate shader
	shader->Activate();
	// Draw Post Proccessing:
	// bind rect vao
	glBindVertexArray(m_VAO);
	// disable depth testing and face culling so we can draw our rect
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	// bind our texture
	m_FBO->BindTex();
	// draw our rect
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// reenable depth testing and face culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

Display::~Display()
{
	delete m_FBO;
	delete m_RBO;
}
