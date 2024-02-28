#version 330 core
layout (location = 0) in vec3 aPos; // positions/ coords
layout (location = 1) in vec3 aNormal; // normals

// get all matricies needed for transformations
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
// get outlining float
uniform float outlining;

void main()
{
	vec3 crntPos = vec3(model * translation * -rotation * scale * vec4(aPos + aNormal * outlining, 1.0f));
	gl_Position = camMatrix * vec4(crntPos, 1.0f);
}