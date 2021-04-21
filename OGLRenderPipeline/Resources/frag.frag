#version 450 core

uniform vec3 color;
uniform sampler2D diffuseTex;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
	FragColor = texture(diffuseTex, TexCoords) * vec4(color, 1.0);
}