#version 330 core
layout (location = 0) out float FragColor;

in vec2 texCoords;

#define KERNEL_SIZE 64
#define RADIUS      0.5f
#define BIAS        0.08f

// import gbuffer
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;
// random noise texture
uniform sampler2D texNoise;

// random samples kernel
uniform vec3 samples[KERNEL_SIZE];
// camera projection
uniform mat4 projection;

// tile noise texture over screen based on screen dimensions divided by noise size
// 1920 / 4 , 1080 / 4
const vec2 noiseScale = vec2(480.0f, 270.0f);

void main()
{
    // sample textures
    vec3 FragPos    = texture(gPosition, texCoords).xyz;
    vec3 Normal     = texture(gNormal, texCoords).xyz * 2.0f - 1.0f;
    //vec4 AlbedoSpec = texture(gAlbedoSpec, texCoords);
    vec3 randomVec  = normalize(texture(texNoise, texCoords * noiseScale).xyz);
    //float Depth     = texture(gDepth, texCoords).r;

    // create a TBN matrix to transform vector from tangent-space to view-space
    vec3 tangent    = normalize(randomVec - Normal * dot(randomVec, Normal));
    vec3 bittangent = cross(Normal, tangent);
    mat3 TBN        = mat3(tangent, bittangent, Normal);

    // iterate over each kernel sample from tangent to view-space
    float occlusion = 0.0f;
    for (int i = 0; i < KERNEL_SIZE; ++i)
    {
        // get sample position
        vec3 samplePos = TBN * samples[i];
        samplePos = FragPos + samplePos * RADIUS;

        // transform sample to screen space
        vec4 offset = vec4(samplePos, 1.0f);
        offset      = projection * offset;      // convert to clip-space
        offset.xyz /= offset.w;                 // perspective divide
        offset.xyz  = offset.xyz * 0.5f + 0.5f; // convert to 0-1 range

        // get depth of the sample by using the xy of the sample as tex pos
        float sampleDepth = texture(gPosition, samplePos.xy).z;

        FragColor = sampleDepth / 100.0f;
        return;

        // create a range check to ensure that fragment only contributes
        // to occlusion factor if its within the sample's radius
        float rangeCheck = smoothstep(0.0f, 1.0f, RADIUS / abs(FragPos.z - sampleDepth));

        // check if sampleDepth is larger than the fragment's depth value
        // if so, add to final total occlusion
        occlusion += (sampleDepth >= samplePos.z + BIAS ? 1.0f : 0.0f) * rangeCheck;
    }
    // normalize the occlusion
    occlusion = 1.0f - (occlusion / KERNEL_SIZE);
    // output occlusion
    FragColor = occlusion;
}