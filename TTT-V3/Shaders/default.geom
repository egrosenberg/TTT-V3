#version 330 core

// specify type of primitive to recieve
layout (triangles) in;
// specify primitive type to output
layout (triangle_strip, max_vertices = 3) out;

// fragment shader outputs
out vec3 Normal;
out vec3 color;
out vec2 texCoord;
out vec4 fragPosLight;

// specify geometry data input struct
in DATA
{
	vec3 Normal;
	vec3 color;
	vec2 texCoord;
	mat4 projection;
	mat4 lightProj;
} data_in[];

void main()
{
	// Vertex 0
	// set pos
	gl_Position = data_in[0].projection * gl_in[0].gl_Position;
	// send output to fragment shader
	Normal = data_in[0].Normal;
	color = data_in[0].color;
	texCoord = data_in[0].texCoord;
    fragPosLight = data_in[0].lightProj * gl_in[0].gl_Position;
	EmitVertex();
	
	// Vertex 1
	// set pos
	gl_Position = data_in[1].projection * gl_in[1].gl_Position;
	// send output to fragment shader
	Normal = data_in[1].Normal;
	color = data_in[1].color;
	texCoord = data_in[1].texCoord;
    fragPosLight = data_in[1].lightProj * gl_in[1].gl_Position;
	EmitVertex();
	
	// Vertex 2
	// set pos
	gl_Position = data_in[2].projection * gl_in[2].gl_Position;
	// send output to fragment shader
	Normal = data_in[2].Normal;
	color = data_in[2].color;
	texCoord = data_in[2].texCoord;
    fragPosLight = data_in[2].lightProj * gl_in[2].gl_Position;
	EmitVertex();
	// Send triangle
	EndPrimitive();
}
