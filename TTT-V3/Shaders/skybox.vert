#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    // calculate vertex position
    vec4 pos = projection * view * vec4(aPos, 1.0f);
    // feed position to ogl with permenent max depth
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    // export texture coords, flipping z because of skybox coord changes
    texCoords = vec3(aPos.x, aPos.y, -aPos.z);
}