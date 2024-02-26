#version 330 core
out vec4 FragColor;

// inputs color for shader
in vec3 color;
// inputs texture coordinates
in vec2 texCoord;
// inputs normal vector and current pos
in vec3 Normal;
in vec3 crntPos;


// use texture sampler uniform
uniform sampler2D tex0;
// use specular map texture
uniform sampler2D tex1;

// use light color uniform
uniform vec4 lightColor;
// use light position
uniform vec3 lightPos;
// use camera position for specular lighting
uniform vec3 camPos;

void main()
{
	// add ambient light
	float ambient = 0.2f;

	// normalize the normal
	vec3 normal = normalize(Normal);
	// calculate light direction
	vec3 lightDirection = normalize(lightPos - crntPos);
	// calculate diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// set maximum intensity of specular light
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	// calculate ammount of specular light at specified angle
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	// calculate final specular value
	float specular = specAmount * specularLight;

	// calculate final pixel color
    FragColor = (texture(tex0, texCoord) * (diffuse + ambient) + texture(tex1, texCoord).r * specular) * lightColor;
}