#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in mat4 aModel;

uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * aModel * vec4(aPos, 1.0);

	Normal = vec3(aModel * vec4(aNormal, 0.0));
	TexCoords = aTexCoords;
	FragPos = vec3(aModel * vec4(aPos, 1.0));
}