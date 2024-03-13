#version 330 core
layout (location = 0) in vec3 aPos; // positions/ coords
layout (location = 1) in vec3 aNormal; // vector normals
layout (location = 2) in vec3 aColor; // Colors
layout (location = 3) in vec2 aTex; // Textures

// outputs the current position
out vec3 crntPos;
// output the normal vectors 
out vec3 Normal;
// outputs the color for the fragment shader
out vec3 color;
// outputs the texture coords for the fragment shader
out vec2 texCoord;

// uniforms for camera 3D
uniform mat4 camMatrix;
// imports transformation matrices from c++
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
    // calculate current position (flipping rotation because of gltf discrepancies)
    crntPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));
    // assigns the normal from vertex data
    Normal = aNormal;
    // Assign color to colors based on layout
    color = aColor;
    // Assign texture based on layout (rotating 90 degrees to account for gltf discrepancies)
    texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;

    // add+mul scale uniform to each position
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}