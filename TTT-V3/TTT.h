#ifndef TTT_H
#define TTT_H

#include <glm/glm.hpp>

#define NAME "TTT-V3"
#define WIN_WIDTH  1920
#define WIN_HEIGHT 1080
#define WIN_RATIO  1.77	// width / height 
#define FOV 45.0f
#define AA_SAMPLES 8
#define GAMMA 2.2f
#define SHADOW_W 2048
#define SHADOW_H 2048
#define SHADOW_FARPLANE 150.0f
#define FONT_HEIGHT 18
#define TERMINAL_ROWS 8
#define TERMINAL_PADDING 10
#define LINE_SPACING 1.2f
#define TERMINAL_BLINK_INTERVAL 0.5f
#define G_BUFFER_DEPTH 3

#define TTT_GENERIC_FUNCTION std::string(void*)

enum class TTTenum
{
    TTT_FRAMEBUFFER             = 0,
    TTT_MULTISAMPLE_FRAMEBUFFER = 1,
    TTT_DEPTH_FRAMEBUFFER       = 2,
    TTT_TEXTURE_2D              = 10,
    TTT_TEXTURE_CUBEMAP         = 11,
    TTT_DIRECTIONAL_LIGHT       = 20,
    TTT_SPOT_LIGHT              = 21,
    TTT_POINT_LIGHT             = 22,
    
    // Data types
    TTT_VOID                    = 100,
    TTT_CHAR,
    TTT_UCHAR,
    TTT_SHORT,
    TTT_INT,
    TTT_UINT,
    TTT_LONG,
    TTT_ULONG,
    TTT_FLOAT,
    TTT_DOUBLE,
    TTT_LDOUBLE,
    TTT_STRING,
    TTT_VEC2F,
    TTT_VEC3F,
    TTT_VEC4F,
    // TTT specific types
    TTT_ENGINE,
    TTT_COG,
    TTT_VAO,
    TTT_VBO,
    TTT_EBO,
    TTT_FBO,
    TTT_RBO,
    TTT_DISPLAY,
    TTT_CUBEMAP,
    TTT_MESH,
    TTT_MODEL,
    TTT_CAMERA,
    TTT_SKYBOX,
    TTT_SHADER,
    TTT_TEXTURE
};

#define POINT_LIGHT 0
#define SPOT_LIGHT  1
#define DIREC_LIGHT 2

struct TTTlight
{
    bool            on;         // track wether to render the light
    unsigned int    type;       // type of light source
    glm::vec4       color;      // light source color
    glm::vec3       position;   // position of light in world coords

    float           a;          // float values that serves a different purpose based on type
    float           b;          // for point: a and b corespond to a and be values in intensity fn
                                // for spot: a and b corespond to outer and inner cones respectively
};

#endif