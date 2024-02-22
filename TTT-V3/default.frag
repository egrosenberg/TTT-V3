#version 330 core
out vec4 FragColor;

// inputs color for shader
in vec3 color;
// inputs texture coordinates
in vec2 texCoord;

// use texture sampler uniform
uniform sampler2D tex0;

void main()
{
	// set color based on input
    FragColor = texture(tex0, texCoord);
}