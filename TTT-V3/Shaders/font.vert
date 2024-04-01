#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    // mul position coords by projection mat4
    gl_Position = projection * vec4(vertex.xy, 0.0f, 1.0f);
    // send tex coords to frag
    TexCoords = vertex.zw;
}