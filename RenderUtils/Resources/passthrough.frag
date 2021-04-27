#version 450 core

uniform sampler2D tex;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
	FragColor = texture(tex, TexCoords);
}