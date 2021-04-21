#version 450 core

uniform sampler2D diffuse;

uniform int camCount;
#define MAX_CAMS 16
uniform sampler2D cameras[MAX_CAMS];

in vec2 TexCoords;
out vec4 FragColor;

vec4 StackTextures(vec4 back, vec4 front) 
{
    return front * front.a + back * (1.0 - front.a);
}

void main()
{
	vec4 result = vec4(0);
	for(int i = 0; i < camCount; i++)
		StackTextures(result, texture(cameras[i], TexCoords));

	// Stacking does not work correctly.
	FragColor = texture(cameras[0], TexCoords);
}