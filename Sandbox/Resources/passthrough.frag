#version 450 core

uniform sampler2D tex;

in vec2 TexCoords;
out vec4 FragColor;

vec4 StackTextures(vec4 back, vec4 front) 
{
    return front * front.a + back * (1.0 - front.a);
}

void main()
{
	FragColor = texture(tex, TexCoords);
}