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


// use texture sampler uniform
uniform sampler2D diffuse0;
// use specular map texture
uniform sampler2D specular0;

// use light color uniform
uniform vec4 lightColor;
// use light position
uniform vec3 lightPos;
// use camera position for specular lighting
uniform vec3 camPos;

vec4 pointLight()
{
	vec3 lightVec = lightPos - crntPos;
	// calculate light intensity with respect to dist
	float dist = length(lightVec);
	float a = 0.1;
	float b = 0.2;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// add ambient light
	float ambient = 0.3f;
	
	// diffuse lighting
	// normalize the normal
	vec3 normal = normalize(Normal);
	// calculate light direction
	vec3 lightDirection = normalize(lightVec);
	// calculate diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	// specular lighting
	// set maximum intensity of specular light
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	// calculate ammount of specular light at specified angle
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	// calculate final specular value
	float specular = specAmount * specularLight;

	// calculate final pixel color
    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
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
	// set maximum intensity of specular light
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	// calculate ammount of specular light at specified angle
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	// calculate final specular value
	float specular = specAmount * specularLight;

	// calculate final pixel color
    return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	// cosine values of cone components
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// add ambient light
	float ambient = 0.2f;

	// diffuse lighting
	// normalize the normal
	vec3 normal = normalize(Normal);
	// calculate light direction
	vec3 lightDirection = normalize(lightPos - crntPos);
	// calculate diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	// set maximum intensity of specular light
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	// calculate ammount of specular light at specified angle
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	// calculate final specular value
	float specular = specAmount * specularLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	// calculate final pixel color
    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}


// Some depth buffer fun, ignoring for now

/*
float near = 0.1f;	// use uniforms for this later?
float far = 100.0f; // use uniforms for this later?

// function to turn logarithmic depth into linear function
float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.5f, float offset = 5.0f)
{
	float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));
}

// function to apply a fog effect to a color
vec4 fog(vec4 col)
{
	float depth = logisticDepth(gl_FragCoord.z);
	return col * (1.0f - depth) + vec4(depth * vec3(0.85f), 1.0f);
}*/

void main()
{
	vec4 col = directionalLight();
	FragColor = col;
}