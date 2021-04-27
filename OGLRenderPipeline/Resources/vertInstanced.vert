#version 450 core

// Define the vertex shape.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

// Instanced part of the vertex.
// This shader essentially draws models in a batch.
layout(location = 3) in mat4 aModel;

// Used to calculate the vertex it's place on screen.
uniform mat4 view;
uniform mat4 projection;

// Forwarded to the fragment shader.
out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

void main()
{
	// Define the resulting matrix.
	gl_Position = projection * view * aModel * vec4(aPos, 1.0);
	// Used in light calculations.
	Normal = vec3(aModel * vec4(aNormal, 0.0));
	// Used to define texture coordinates.
	TexCoords = aTexCoords;
	// Used to define the fragment position.
	FragPos = vec3(aModel * vec4(aPos, 1.0));
}