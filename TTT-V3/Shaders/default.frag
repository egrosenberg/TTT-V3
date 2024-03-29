#version 330 core
out vec4 FragColor;

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


// use texture sampler uniform
uniform sampler2D diffuse0;
// use specular map texture
uniform sampler2D specular0;
// use shadowmap texture
uniform sampler2D shadowMap;
uniform samplerCube shadowCubeMap;
// uniform to check if we only want to render shadow map
uniform bool shadowOnly;
// use light color uniform
uniform vec4 lightColor;
// use light position
uniform vec3 lightPos;
// use camera position for specular lighting
uniform vec3 camPos;
// use far plane value
uniform float farPlane;
// check which light type to render
uniform int lightMode;

vec4 pointLight()
{
    vec3 lightVec = lightPos - crntPos;
    // calculate light intensity with respect to dist
    float dist = length(lightVec);
    float a = 0.003;
    float b = 0.2;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    // add ambient light
    float ambient = 0.01f;
    
    // diffuse lighting
    // normalize the normal
    vec3 normal = normalize(Normal);
    // calculate light direction
    vec3 lightDirection = normalize(lightVec);
    // calculate diffuse lighting
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    
    // specular lighting
    float specular = 0.0f;
    // only apply specular light if there is already light
    if (diffuse != 0.0f)
    {
        // set maximum intensity of specular light
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(camPos - crntPos);
        vec3 reflectionDirection = reflect(-lightDirection, normal);

        // calculate halfway vector
        vec3 halfwayVec = normalize(viewDirection + lightDirection);

        // calculate ammount of specular light at specified angle
        float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
        // calculate final specular value
        specular = specAmount * specularLight;
    }
    
    // Calculate shadow
    float shadow = 0.0f;
    vec3 fragToLight = crntPos - lightPos;
    float currentDepth = length(fragToLight);
    float bias = max(0.5f * (1.0f - dot(normal, lightDirection)), 0.0005);
    // soften shadow
    int sampleRadius = 2;
    float offset = 0.02;
    for ( int z = -sampleRadius; z <= sampleRadius; ++z)
    {
        for ( int  y = -sampleRadius; y<= sampleRadius; ++y)
        {
            for (int x = -sampleRadius; x <= sampleRadius; ++x)
            {
                float closestDepth = texture(shadowCubeMap, fragToLight + vec3(x, y, z) * offset).r;
                closestDepth *= farPlane;
                if (currentDepth > closestDepth + bias)
                {
                    shadow += 1.0f;
                }
            }
        }
    }
    shadow /= pow((sampleRadius * 2 + 1), 3);
    // display just shadows if uniform is toggled
    if (shadowOnly)
    {
        return vec4(vec3(1.0f - shadow), 1.0f);
    }
    // calculate final pixel color
    return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten + ambient) + (texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten)) * lightColor;
}

vec4 directionalLight()
{
    // add ambient light
    float ambient = 0.2f;
    
    // diffuse lighting
    // normalize the normal
    vec3 normal = normalize(Normal);
    // calculate light direction
    vec3 lightDirection = normalize(vec3(0.5f, 1.0f, 1.0f)); // WE CAN USE A UNIFORM FOR THIS LATER
    // calculate diffuse lighting
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    
    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        // set maximum intensity of specular light
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(camPos - crntPos);
        vec3 reflectionDirection = reflect(-lightDirection, normal);

        // calculate halfway vector
        vec3 halfwayVec = normalize(viewDirection + lightDirection);

        // calculate ammount of specular light at specified angle
        float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
        // calculate final specular value
        specular = specAmount * specularLight;
    }

    // shadow calculations
    float shadow = 0.0f;
    // set light coords to cull space
    vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        // Get from [-1, 1] range to [0, 1] range just like the shadow map
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        // Prevents shadow acne
        float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);

        // Smoothens out the shadows
        int sampleRadius = 8;
        vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
        for(int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for(int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                {
                    shadow += 1.0f;
                }
            }    
        }
        // Get average shadow
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }
    // display just shadows if uniform is toggled
    if (shadowOnly)
    {
        return vec4(vec3(1- shadow), 1.0f);
    }

    // calculate final pixel color
    return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) + ambient) + texture(specular0, texCoord).r * specular*(1.0f - shadow)) * lightColor;
}

vec4 spotLight()
{
    // cosine values of cone components
    float outerCone = 0.70f;
    float innerCone = 0.95f;

    // add ambient light
    float ambient = 0.15f;

    // diffuse lighting
    // normalize the normal
    vec3 normal = normalize(Normal);
    // calculate light direction
    vec3 lightDirection = normalize(lightPos - crntPos);
    // calculate diffuse lighting
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        // set maximum intensity of specular light
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(camPos - crntPos);
        vec3 halfwayVec = normalize(viewDirection + lightDirection);
        // calculate ammount of specular light at specified angle
        float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
        // calculate final specular value
        specular = specAmount * specularLight;
    }

    // calculates the intensity of the pos based on its angle compared to center of cone
    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    // shadow calculations
    float shadow = 0.0f;
    // set light coords to cull space
    vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        // Get from [-1, 1] range to [0, 1] range just like the shadow map
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        // Prevents shadow acne
        float bias = max(0.00025f * (1.0f - dot(normal, lightDirection)), 0.000005f);

        // Smoothens out the shadows
        int sampleRadius = 2;
        vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
        for(int y = -sampleRadius; y <= sampleRadius; y++)
        {
             for(int x = -sampleRadius; x <= sampleRadius; x++)
             {
                 float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                 if (currentDepth > closestDepth + bias)
                 {
                     shadow += 1.0f;
                 }
             }
         }
        // Get average shadow
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }
    // display just shadows if uniform is toggled
    if (shadowOnly)
    {
        return vec4(vec3(1.0f - shadow), 1.0f);
    }

    // calculate final pixel color
    return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * lightColor;
}

void main()
{
    vec4 col = vec4(1.0f);
    if (lightMode == 22)
    {
        col = pointLight();
    }
    else if (lightMode == 20)
    {
        col = directionalLight();
    }
    else if (lightMode == 21)
    {
        col = spotLight();
    }
    FragColor = col;
}