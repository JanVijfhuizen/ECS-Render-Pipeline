#version 450 core

uniform sampler2D diffuse;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
	vec4 res = texture(diffuse, TexCoords);
	FragColor = vec4(vec3(1) - vec3(res), res.a);
}