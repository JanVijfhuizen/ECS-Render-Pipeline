#version 450 core

uniform sampler2D diffuse;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
	FragColor = texture(diffuse, TexCoords);
}