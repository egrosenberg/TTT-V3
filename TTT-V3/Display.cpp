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

Display::Display()
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


	// initalize frame buffer
	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	// Create a texture to store the framebuffer in
	glGenTextures(1, &m_Tex);
	glBindTexture(GL_TEXTURE_2D, m_Tex);
	// configure texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// set upscaling mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Prevent edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// attach the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Tex, 0);

	// init RBO
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	// configured RBO storage          // store stencil and depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIN_WIDTH, WIN_HEIGHT);
	// attach RBO to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	// error checking
	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR (FRAMEBUFFER): " << fboStatus << std::endl;
	}
}

void Display::Bind(GLuint internalW, GLuint internalH)
{
	// set viewport to internal resolution
	glViewport(0, 0, internalW, internalH);
	// bind frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void Display::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Draws the contents of the fbo to the screen, scaled to current window size
void Display::Draw(Shader *shader, GLFWwindow *window)
{
	scaleViewport(window);
	shader->Activate();
	// Draw Post Proccessing:
	// bind rect vao
	glBindVertexArray(m_VAO);
	// disable depth testing and face culling so we can draw our rect
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	// bind our texture
	glBindTexture(GL_TEXTURE_2D, m_Tex);
	// draw our rect
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// reenable depth testing and face culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

Display::~Display()
{

}
