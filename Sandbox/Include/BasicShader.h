#pragma once
#include "Shaders/CeShader.h"

class BasicShader : public rpi::CeShader
{
public:
	explicit BasicShader(GLuint program);

	void Use(int32_t index, glm::vec3 eye, const glm::mat4& view, 
	         const glm::mat4& projection) override;

private:
	GLuint _model = 0;
};
