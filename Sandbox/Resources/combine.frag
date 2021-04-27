#version 450 core

uniform sampler2D a;
uniform sampler2D b;

in vec2 TexCoords;
out vec4 FragColor;

vec4 StackTextures(vec4 back, vec4 front) 
{
    return front * front.a + back * (1.0 - front.a);
}

void main()
{
	vec4 aRes = texture(a, TexCoords);
	vec4 bRes = texture(b, TexCoords);

	FragColor = StackTextures(aRes, bRes);
}