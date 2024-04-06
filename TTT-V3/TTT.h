#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NAME "TTT-V3"
#define WIN_WIDTH  1920
#define WIN_HEIGHT 1080
#define WIN_RATIO  1.77	// width / height 
#define FOV 45.0f
#define AA_SAMPLES 8
#define GAMMA 2.2f
#define SHADOW_W 2048
#define SHADOW_H 2048
#define SHADOW_FARPLANE 100.0f
#define FONT_HEIGHT 28
#define TERMINAL_ROWS 8
#define TERMINAL_PADDING 15
#define LINE_SPACING 1.2f
#define TERMINAL_BLINK_INTERVAL 0.5f

#define TTT_GENERIC_FUNCTION void(void*)

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
    TTT_VEC3,
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

#endif