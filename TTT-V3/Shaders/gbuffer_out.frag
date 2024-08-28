#version 430 core
out vec4 FragColor;

in vec2 texCoords;

#define POINT_LIGHT 0
#define SPOT_LIGHT  1
#define DIREC_LIGHT 2

// struct for lights
struct lightSource
{
    bool            on;         // track wether to render the light
    unsigned int    type;       // type of light source
    vec4            color;      // light source color
    vec3            position;   // position of light in world coords

    float           a;          // float values that serves a different purpose based on type
    float           b;          // for point: a and b corespond to a and be values in intensity fn
                                // for spot: a and b corespond to outer and inner cones respectively
};

struct diffuse_specular
{
    vec3 diffuse;
    vec3 specular;
};

// import gbuffer
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// light uniforms
uniform int nLights;
uniform lightSource light[7];

// shadow map
uniform samplerCube shadowCubeMap[7];
uniform sampler2D shadowMap[7];
// use light projection for shadows (spot / directional)
uniform mat4 lightProj[7];
// use camera position for specular lighting
uniform vec3 camPos;
// use far plane value
uniform float farPlane;
// check if we should only render lighting
uniform bool lightingOnly;
// get ambient value from main program
uniform float ambientVal;

uniform mat4 camMatrix;

/**
 * Calculate intensity of fragment based on a point light
 * 
 * @param lightVec: vec3 from light position to frag pos (lightPos - fragPos)
 * @return inensity value at fragment for light
 */
float pointLightInten(vec3 lightVec, float a, float b)
{
    float dist = length(lightVec);
    return 1.0f / (a * dist*dist + b * length(lightVec) + 1.0f);
}

/**
 * Calculate the ammount of specular light for fragment
 * 
 * @param crntPos: vec3 current position of the fragment in world space
 * @param lightDirection: normalized vector from light position to frag pos
 * @param normal: vec3 normalized normal of frag
 * @param specular: float value from specular map
 * @param ammount of specular light for fragment
 */
float specular(vec3 crntPos, vec3 lightDirection, vec3 normal, float specular)
{
    // set maximum specular value
    float specularLight = 0.50f;

    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    // calculate haflway vec
    vec3 halfwayVec = normalize(viewDirection + lightDirection);

    // calculate ammount of specular light based on angle
    float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);

    // calculate and return final specular value
    return specAmount * specularLight * specular;
}

/**
 * Calculates shadow value for a point light
 * 
 * @param crntPos: vec3 of 3d position of fragment
 * @param lightDirection: normalized vector from light position to frag position
 * @param normal: vec3 normalized normal of 
 * @param i: index of shadowcubemap sampler
 * @return shadow value for fragment
 */
float pointLightShadow(vec3 crntPos, vec3 lightDirection, vec3 normal, int i)
{

    // Calculate shadow
    float shadow = 0.0f;

    vec3 fragToLight = crntPos - light[i].position;
    float currentDepth = length(fragToLight);
    if (currentDepth >= farPlane)
    {
        return 0;
    }
    float bias = max(0.5f * (1.0f - dot(normal, lightDirection)), 0.00025);
    // soften shadow
    int sampleRadius = 2;
    float offset = 0.04;
    for ( int z = -sampleRadius; z <= sampleRadius; ++z)
    {
        for ( int  y = -sampleRadius; y<= sampleRadius; ++y)
        {
            for (int x = -sampleRadius; x <= sampleRadius; ++x)
            {
                float closestDepth = texture(shadowCubeMap[i], fragToLight + vec3(x, y, z) * offset).r;
                closestDepth *= farPlane;
                if (currentDepth > closestDepth + bias)
                {
                    shadow += 1.0f;
                }
            }
        }
    }
    shadow /= pow((sampleRadius * 2 + 1), 3);
    return shadow;
}

/**
 * Calculates diffuse and specular light values for a point light
 * 
 * @param FragPos: position of fragment
 * @param Normal: normalized normal of fragment
 * @param Specular: specular map value for fragment
 * @param i: index of light source in lights uniform
 */
diffuse_specular pointLight(vec3 FragPos, vec3 Normal, float Specular, int i)
{
        // calculate vectors for distance and direction
        vec3 lightVec = light[i].position - FragPos;
        
        float dist = length(lightVec);
        float intensity = 1.0f / (light[i].a * dist*dist + light[i].b * dist + 1.0f);

        // check if we need to bother continuing calculations
        if (intensity < 0.01f)
        {
            diffuse_specular ret = {vec3(0.0f), vec3(0.0f)};
            return ret;
        }

        vec3 lightDirection = normalize(lightVec);
        // calculate intensity
        // calculate shadow intesity
        float shadow = pointLightShadow(FragPos, lightDirection, Normal, i);
        float shadowInv = 1.0f - shadow;
        // calculate base diffuse value from direction
        float diffuse = max(dot(Normal, lightDirection), 0.0f);;

        // calculate final diffuse and specular values
        diffuse_specular ret = {vec3(0.0f), vec3(0.0f)};

        ret.diffuse = vec3(max(diffuse * intensity * shadowInv, 0)) * light[i].color.rgb;
        if (diffuse > 0)
        {
            ret.specular = vec3(specular(FragPos, lightVec, Normal, Specular) * intensity * shadowInv) * light[i].color.rgb;
        }
        return ret;
}

/**
 * Calculates shadow value for a spot light
 * 
 * @param FragPos: vec3 of 3d position of fragment
 * @param lightDirection: normalized vector from light position to frag position
 * @param Normal: vec3 normalized normal of 
 * @param i: index of shadowcubemap sampler
 * @return shadow value for fragment
 */
float spotLightShadow(vec3 FragPos, vec3 lightDirection, vec3 Normal, int i)
{
    // shadow calculations
    float shadow = 0.0f;
    // set light coords to cull space
    vec4 fragPosLight = lightProj[i] * vec4(FragPos, 1.0f);
    vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        // Get from [-1, 1] range to [0, 1] range just like the shadow map
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        // Prevents shadow acne
        float bias = max(0.00025f * (1.0f - dot(Normal, lightDirection)), 0.000005f);

        // Smoothens out the shadows
        int sampleRadius = 2;
        vec2 pixelSize = 1.0 / textureSize(shadowMap[i], 0);
        for(int y = -sampleRadius; y <= sampleRadius; y++)
        {
             for(int x = -sampleRadius; x <= sampleRadius; x++)
             {
                 float closestDepth = texture(shadowMap[i], lightCoords.xy + vec2(x, y) * pixelSize).r;
                 if (currentDepth > closestDepth + bias)
                 {
                     shadow += 1.0f;
                 }
             }
         }
        // Get average shadow
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }
    return shadow;
}

/**
 * Calculates diffuse and specular light values for a spot light
 * 
 * @param FragPos: position of fragment
 * @param Normal: normalized normal of fragment
 * @param Specular: specular map value for fragment
 * @param i: index of light source in lights uniform
 */
diffuse_specular spotLight(vec3 FragPos, vec3 Normal, float Specular, int i)
{
    
    vec3 lightVec = light[i].position - FragPos;
    vec3 lightDirection = normalize(lightVec);

    float diffuse = max(dot(Normal, lightDirection), 0.0f);
    
    // specular lighting
    float specular = specular(FragPos, lightDirection, Normal, Specular);
    
    // calculates the intensity of the pos based on its angle compared to center of cone
    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float inten = clamp((angle - light[i].a) / (light[i].b - light[i].a), 0.0f, 1.0f);
    
    // calculate shadow intesity
    float shadow = spotLightShadow(FragPos, lightDirection, Normal, i);
    float shadowInv = 1.0f - shadow;

    // calculate final diffuse and specular values
    diffuse_specular ret = {vec3(0.0f), vec3(0.0f)};

    ret.diffuse = vec3(diffuse * inten * shadowInv) * light[i].color.rgb;
    if (diffuse > 0)
    {
        ret.specular = vec3(specular * Specular * inten * shadowInv) * light[i].color.rgb;
    }

    return ret;
}

/**
 * Calculates shadow value for a directional light
 * 
 * @param FragPos: vec3 of 3d position of fragment
 * @param lightDirection: normalized vector from light position to frag position
 * @param Normal: vec3 normalized normal of 
 * @param i: index of shadowcubemap sampler
 * @return shadow value for fragment
 */
float directionalLightShadow(vec3 FragPos, vec3 lightDirection, vec3 Normal, int i)
{
    float shadow = 0.0f;
    // set light coords to cull space
    vec4 fragPosLight = lightProj[i] * vec4(FragPos, 1.0f);
    vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        // Get from [-1, 1] range to [0, 1] range just like the shadow map
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        // Prevents shadow acne
        float bias = max(0.0025f * (1.0f - dot(Normal, lightDirection)), 0.0005f);

        // Smoothens out the shadows
        int sampleRadius = 2;
        vec2 pixelSize = 1.0 / textureSize(shadowMap[i], 0);
        for(int y = -sampleRadius; y <= sampleRadius; y++)
        {
             for(int x = -sampleRadius; x <= sampleRadius; x++)
             {
                 float closestDepth = texture(shadowMap[i], lightCoords.xy + vec2(x, y) * pixelSize).r;
                 if (currentDepth > closestDepth + bias)
                 {
                     shadow += 1.0f;
                 }
             }
         }
        // Get average shadow
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }
    return shadow;
}
/**
 * Calculates diffuse and specular light values for a directional light
 * 
 * @param FragPos: position of fragment
 * @param Normal: normalized normal of fragment
 * @param Specular: specular map value for fragment
 * @param i: index of light source in lights uniform
 */
diffuse_specular directionalLight(vec3 FragPos, vec3 Normal, float Specular, int i)
{
    vec3 lightVec = light[i].position;
    vec3 lightDirection = normalize(lightVec);

    // calculate diffuse lighting
    float diffuse = max(dot(Normal, lightDirection), 0.0f);

    
    // specular lighting
    float specular = specular(FragPos, lightDirection, Normal, Specular);

    float shadow = directionalLightShadow(FragPos, lightDirection, Normal, i);
    float shadowInv = 1.0f - shadow;
    
    // calculate final diffuse and specular values
    diffuse_specular ret = {vec3(0.0f), vec3(0.0f)};

    ret.diffuse = vec3(diffuse * shadowInv) * light[i].color.rgb;
    if (diffuse > 0)
    {
        ret.specular = vec3(specular * Specular * shadowInv) * light[i].color.rgb;
    }

    return ret;
}

void main()
{
    vec3 FragPos = texture(gPosition, texCoords).rgb;
    vec3 Normal = texture(gNormal, texCoords).rgb;
    vec4 AlbedoSpec = texture(gAlbedoSpec, texCoords);
    vec3 Diffuse = lightingOnly ? vec3(1.0f) : AlbedoSpec.rgb;
    float Specular = AlbedoSpec.r;

    // calculate total light from light sources
    vec3 diffuseTotal = vec3(0.0f);
    vec3 specularTotal = vec3(0.0f);
    for (int i = 0; i < nLights; ++i)
    {
        if (!light[i].on)
        {
            continue;
        }
        diffuse_specular cVals = {vec3(0.0f), vec3(0.0f)};
        if (light[i].type == POINT_LIGHT)
        {
            cVals = pointLight(FragPos, Normal, Specular, i);
        }
        else if (light[i].type == SPOT_LIGHT)
        {
            cVals = spotLight(FragPos, Normal, Specular, i);
        }
        else if (light[i].type == DIREC_LIGHT)
        {
            cVals = directionalLight(FragPos, Normal, Specular, i);
        }
        diffuseTotal += cVals.diffuse;
        specularTotal += cVals.specular;
    }

    vec3 col = Diffuse * (diffuseTotal + ambientVal);
    col += specularTotal;

    FragColor = vec4(col, 1.0f);
}