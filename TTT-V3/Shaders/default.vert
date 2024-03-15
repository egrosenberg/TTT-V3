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

// output for geom shader
out DATA
{
	vec3 Normal;
	vec3 color;
	vec2 texCoord;
	mat4 projection;
} data_out;

// uniforms for camera 3D
uniform mat4 camMatrix;
// imports transformation matrices from c++
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
    // calculate current position
    gl_Position = model * translation * rotation * scale * vec4(aPos, 1.0f);
    // output variables we pass through
    data_out.Normal = aNormal;
    data_out.color = aColor;
    // rotating 90 degrees to account for gltf discrepancies
    data_out.texCoord = texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
    data_out.projection = camMatrix;
}