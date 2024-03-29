#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

// manually calculate depth from light position
void main()
{
    gl_FragDepth = length(FragPos.xyz - lightPos) / farPlane;
}