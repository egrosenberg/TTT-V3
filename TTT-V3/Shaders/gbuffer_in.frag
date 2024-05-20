#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out float gDepth;


// inputs current position
in vec3 crntPos;
// inputs normal vector
in vec3 Normal;
// inputs color for shader
in vec3 color;
// inputs texture coordinates
in vec2 texCoord;
// input light/shadow pos
in vec4 fragPosLight;
in mat4 projection;


// use texture sampler uniform
uniform sampler2D diffuse0;
// use specular map texture
uniform sampler2D specular0;

void main()
{
    // store the frag position in gbuffer position
    vec4 viewPos = projection * vec4(crntPos, 1.0f);
    //viewPos.xyz /= viewPos.w;
    gPosition.xy = viewPos.xy;// * 0.5f +0.5f;
    gPosition.z = viewPos.z;
    //gPosition = crntPos;
    // Normalize normal because other operations may have messed with it
    gNormal = normalize(Normal);
    // store diffuse color in g-buffer
    gAlbedoSpec.rgb = texture(diffuse0, texCoord).rgb;
    // store specular intensity as alpha
    gAlbedoSpec.a = texture(specular0, texCoord).r;
    gDepth = viewPos.z / 100.0f;
}