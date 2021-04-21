#version 450 core

struct Material 
{
	vec3 color;
	int specularity;

    sampler2D diffuse;
	sampler2D normal;
    sampler2D specular;
};

struct PtLight
{
	vec3 diffuse;
	vec3 pos;

	float constant;
	float linear;
	float quadratic;
};

struct DirLight
{
	vec3 diffuse;
	vec3 dir;
};

uniform vec3 viewPos;
uniform Material mat;

// Ambient light.
uniform vec3 ambient;

// Point lights.
uniform int ptCount;
#define MAX_POINT_LIGHTS 4
uniform PtLight ptLights[MAX_POINT_LIGHTS];

// Directional lights.
uniform int dirCount;
#define MAX_DIR_LIGHTS 4
uniform DirLight dirLights[MAX_DIR_LIGHTS];

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.dir);

    // Diffuse shading.
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading.
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.specularity);

    // Combine results.
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.diffuse, TexCoords));
    vec3 specular = spec * vec3(texture(mat.specular, TexCoords));

    return diffuse + specular;
}

vec3 CalcPointLight(PtLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - FragPos);

    // Diffuse shading.
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading.
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.specularity);

    // Attenuation.
    float distance = length(light.pos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
		light.quadratic * (distance * distance));  

    // Combine results.
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.diffuse, TexCoords));
    vec3 specular = spec * vec3(texture(mat.specular, TexCoords));

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
}

void main()
{
	// Shared calculations.
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// Add point lights.
	vec3 lightMod = vec3(0, 0, 0);
	for(int i = 0; i < ptCount; i++)
        lightMod += CalcPointLight(ptLights[i], norm, viewDir);
    for(int i = 0; i < dirCount; i++)
        lightMod += CalcDirLight(dirLights[i], norm, viewDir);

	vec4 shadingRes = vec4((ambient + lightMod) * mat.color, 1.0);
	FragColor = texture(mat.diffuse, TexCoords) * shadingRes;
}