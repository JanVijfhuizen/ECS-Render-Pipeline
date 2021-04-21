#version 450 core

struct Material 
{
	vec3 color;

    sampler2D diffuse;
	sampler2D normal;
    sampler2D specular;
};

struct PtLight
{
	vec3 diffuse;
	vec3 pos;
};

struct DirLight
{
	vec3 diffuse;
	vec3 dir;
};

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

void main()
{
	// Shared calculations.
	vec3 norm = normalize(Normal);

	// Add point lights.
	vec3 ptLightVal = vec3(0, 0, 0);
	for(int i = 0; i < ptCount; i++)
	{
		vec3 dir = normalize(ptLights[i].pos - FragPos);  
		float diff = max(dot(norm, dir), 0.0);
		vec3 diffuse = diff * ptLights[i].diffuse;
		ptLightVal += diffuse;
	}

	vec4 shadingRes = vec4((ambient + ptLightVal) * mat.color, 1.0);
	FragColor = texture(mat.diffuse, TexCoords) * shadingRes;
}