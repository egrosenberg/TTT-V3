#version 330 core
layout (location = 0) in vec3 aPos; // positions/ coords
layout (location = 1) in vec3 aColor; // Colors
layout (location = 2) in vec2 aTex; // Textures

// outputs the color for the fragment shader
out vec3 color;

// outputs the texture coords for the fragment shader
out vec2 texCoord;

// use scale uniform
uniform float scale;

void main()
{
    // add+mul scale uniform to each position
    gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
    // Assign color to colors based on layout
    color = aColor;
    // Assign texture based on layout
    texCoord = aTex;
}