#version 450 core

// Textures to combine.
uniform sampler2D a;
uniform sampler2D b;

in vec2 TexCoords;
out vec4 FragColor;

// Put one texture on top of the other one.
vec4 StackTextures(vec4 back, vec4 front) 
{
    return front * front.a + back * (1.0 - front.a);
}

void main()
{
	// Stack the textures on top of eachother.
	vec4 aRes = texture(a, TexCoords);
	vec4 bRes = texture(b, TexCoords);

	FragColor = StackTextures(aRes, bRes);
}