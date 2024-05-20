#ifndef MAIN_H
#define MAIN_H

#include "TTT.h"
#include "Model.h"
#include "Display.h"
#include "CubeMap.h"
#include "Skybox.h"
#include "Font.h"
#include "Terminal.h"
#include "TTTengine.h"
#include "Shadowmap.h"
#include "Gbuffer.h"
#include "Scene.h"

#include <glad/glad.h>
#include <list>
#include <random>

std::string readFile(const char* filename);

#endif