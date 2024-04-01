#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    // sample the texture to find if we should draw
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, TexCoords).r);
    vec4 col = vec4(textColor, 1.0) * sampled;
    FragColor = col;
}