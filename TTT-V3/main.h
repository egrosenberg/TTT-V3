#ifndef MAIN_H
#define MAIN_H

#include "Model.h"
#include "Display.h"
#include "CubeMap.h"
#include "Skybox.h"
#include <glad/glad.h>

#define NAME "TTT-V3"
#define WIN_WIDTH  1920
#define WIN_HEIGHT 1080
#define WIN_RATIO  1.77	// width / height 
#define FOV 45.0f

std::string readFile(const char* filename);

#endif