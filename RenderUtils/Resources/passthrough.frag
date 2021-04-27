#version 450 core

// Basic shader used to draw a texture on a mesh. 
uniform sampler2D tex;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
	FragColor = texture(tex, TexCoords);
}