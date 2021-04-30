#version 450 core

#pragma region Structs
struct Material
{
	vec3 color;
	float specularity;

    sampler2D diffuseTex;
	sampler2D normalTex;
    sampler2D specularityTex;
};

struct Fallof
{
	float constant;
	float linear;
	float quadratic;
};

struct PtLight
{
	Fallof fallof;
	vec3 diffuse;
	vec3 pos;
};

struct DirLight
{
	vec3 diffuse;
	vec3 dir;
};

struct SpotLight
{
	Fallof fallof;
    float cutOff;
    vec3 diffuse;
    vec3 pos;
    vec3 dir;
};
#pragma endregion

uniform vec3 viewPos;
uniform Material mat;

// Ambient light.
uniform vec3 ambient;

// Point lights.
#define MAX_POINT_LIGHTS 32
uniform int ptCount;
uniform PtLight ptLights[MAX_POINT_LIGHTS];

// Directional lights.
#define MAX_DIR_LIGHTS 32
uniform int dirCount;
uniform DirLight dirLights[MAX_DIR_LIGHTS];

// Spot lights.
#define MAX_SPOT_LIGHTS 32
uniform int spotCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

out vec4 FragColor;

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
    float attenuation = 1.0 / (light.fallof.constant + light.fallof.linear * distance + 
		light.fallof.quadratic * (distance * distance));  

    // Combine results.
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.diffuseTex, TexCoords));
    vec3 specular = spec * vec3(texture(mat.specularityTex, TexCoords));

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light)
{
    vec3 lightDir = normalize(light.pos - FragPos);
    float theta = dot(lightDir, normalize(-light.dir));
    if(theta < light.cutOff) 
        return vec3(0);

    // Diffuse shading.
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(mat.diffuseTex, TexCoords).rgb;  
        
    // Specular shading.
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.specularity);
    vec3 specular = spec * texture(mat.specularityTex, TexCoords).rgb;  
        
    // Attenuation.
    float distance = length(light.pos - FragPos);
    float attenuation = 1.0 / (light.fallof.constant + light.fallof.linear * distance + 
        light.fallof.quadratic * (distance * distance));    

    diffuse *= attenuation;
    specular *= attenuation;   
    
    return diffuse + specular;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.dir);

    // Diffuse shading.
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading.
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.specularity);

    // Combine results.
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.diffuseTex, TexCoords));
    vec3 specular = spec * vec3(texture(mat.specularityTex, TexCoords));

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
    // Add spot lights.
    for(int i = 0; i < spotCount; i++)
        lightMod += CalcSpotLight(spotLights[i]);
    // Add directional lights.
    for(int i = 0; i < dirCount; i++)
        lightMod += CalcDirLight(dirLights[i], norm, viewDir);

	vec4 shadingRes = vec4((ambient + lightMod) * mat.color, 1.0);
	FragColor = texture(mat.diffuseTex, TexCoords) * shadingRes;
}