#pragma once
#include "Shaders/CeShader.h"

// Used to draw a non-instanced mesh by forwarding it's transform to the shader.
class BasicShader : public rpi::CeShader
{
public:
	explicit BasicShader(GLuint program);

	// Once activated, the following meshes will be drawn using this shader.
	void Use(int32_t index, glm::vec3 eye, const glm::mat4& view, 
	         const glm::mat4& projection) override;

private:
	GLuint _model = 0;
};
