#version 330 core
layout (location = 0) in vec3 aPos; // positions/ coords
layout (location = 1) in vec3 aColor; // Colors
layout (location = 2) in vec2 aTex; // Textures
layout (location = 3) in vec3 aNormal; // vector normals

// outputs the color for the fragment shader
out vec3 color;

// outputs the texture coords for the fragment shader
out vec2 texCoord;

// output the normal vectors and current pos
out vec3 Normal;
out vec3 crntPos;

// uniforms for camera 3D
uniform mat4 camMatrix;

uniform mat4 model;

void main()
{
    // calculate current position
    crntPos = vec3(model * vec4(aPos, 1.0f));

    // add+mul scale uniform to each position
    gl_Position = camMatrix * vec4(crntPos, 1.0);
    // Assign color to colors based on layout
    color = aColor;
    // Assign texture based on layout
    texCoord = aTex;
    // assigns the normal from vertex data
    Normal = aNormal;
}