#version 330 core

// specify type of primitive to recieve
layout (triangles) in;
// specify primitive type to output
layout (triangle_strip, max_vertices = 3) out;

// fragment shader outputs
out vec3 crntPos;
out vec3 Normal;
out vec3 color;
out vec2 texCoord;
out vec4 fragPosLight;
out float depthValue;
out mat4 projection;

// specify geometry data input struct
in DATA
{
    vec3 crntPos;
    vec3 Normal;
    vec3 color;
    vec2 texCoord;
    mat4 projection;
} data_in[];

void main()
{
    // Vertex 0
    // set pos
    gl_Position = data_in[0].projection * gl_in[0].gl_Position;
    // send output to fragment shader
    crntPos = data_in[0].crntPos;
    Normal = data_in[0].Normal;
    color = data_in[0].color;
    texCoord = data_in[0].texCoord;
    projection = data_in[0].projection;
    EmitVertex();
    
    // Vertex 1
    // set pos
    gl_Position = data_in[1].projection * gl_in[1].gl_Position;
    // send output to fragment shader
    crntPos = data_in[1].crntPos;
    Normal = data_in[1].Normal;
    color = data_in[1].color;
    texCoord = data_in[1].texCoord;
    projection = data_in[1].projection;
    EmitVertex();
    
    // Vertex 2
    // set pos
    gl_Position = data_in[2].projection * gl_in[2].gl_Position;
    // send output to fragment shader
    crntPos = data_in[2].crntPos;
    Normal = data_in[2].Normal;
    color = data_in[2].color;
    texCoord = data_in[2].texCoord;
    projection = data_in[2].projection;
    EmitVertex();
    // Send triangle
    EndPrimitive();
}
