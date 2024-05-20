#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;
uniform bool blackSkybox;

void main()
{
    FragColor = blackSkybox ? vec4(vec3(0.0f), 1.0f) : texture(skybox, texCoords);
}