#version 450 core

uniform vec3 color;
uniform sampler2D diffuseTex;

// Lighting.
uniform vec3 ambient;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
	vec3 postAmbient = ambient * color;
	FragColor = texture(diffuseTex, TexCoords) * vec4(postAmbient, 1.0);
}