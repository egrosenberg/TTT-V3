#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightProj;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
    gl_Position = lightProj *  model * translation * rotation * scale * vec4(aPos, 1.0);
}