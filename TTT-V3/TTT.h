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
#define FONT_HEIGHT 36
#define TERMINAL_ROWS 8
#define TERMINAL_PADDING 15
#define LINE_SPACING 1.2f
#define TERMINAL_BLINK_INTERVAL 0.5f
namespace TTT
{
    enum TTTenum
    {
        TTT_FRAMEBUFFER = 0,
        TTT_MULTISAMPLE_FRAMEBUFFER = 1,
        TTT_DEPTH_FRAMEBUFFER = 2,
        TTT_TEXTURE_2D = 10,
        TTT_TEXTURE_CUBEMAP = 11,
        TTT_DIRECTIONAL_LIGHT = 20,
        TTT_SPOT_LIGHT = 21,
        TTT_POINT_LIGHT = 22,
        TTT_INT = 30,
        TTT_FLOAT = 31,
        TTT_STRING = 32,
        TTT_VEC3 = 33
    };
}

#endif