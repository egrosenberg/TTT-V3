#version 330 core

// input triangles
layout (triangles) in;
// output lines
layout (line_strip, max_vertices = 4) out;

// specify geometry data input struct
in DATA
{
	vec3 Normal;
	vec3 color;
	vec2 texCoord;
	mat4 projection;
} data_in[];

void main()
{
	// Draw lines between each vertex
	gl_Position = data_in[0].projection * gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = data_in[1].projection * gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = data_in[2].projection * gl_in[2].gl_Position;
	EmitVertex();
	gl_Position = data_in[0].projection * gl_in[0].gl_Position;
	EmitVertex();
	EndPrimitive();
}