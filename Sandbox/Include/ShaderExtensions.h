#pragma once
#include "ModularShader.h"

class ModelFwd final : public ShaderExt
{
public:
	void Init(GLuint program) override;
	void Use(int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection) override;
	
private:
	GLuint _model = 0;
};
