#pragma once
#include "Shaders/ModularShader.h"

// Extension that forwards the (baked) transform matrix to the shader.
class ModelFwd final : public rpi::ShaderExt
{
public:
	void Init(GLuint program) override;
	// Once activated, the following meshes will be drawn using this shader.
	void Use(int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection) override;
	
private:
	GLint _model = -1;
};

// Extension that forwards a basic material.
class MatFwd final : public rpi::ShaderExt
{
public:
	void Init(GLuint program) override;
	void Use(int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection) override;

private:
	GLint _color = -1;
	GLint _specularity = -1;
};

// Extension that forwards the diffuse, normal and specular lightmaps.
class LightmapFwd final : public rpi::ShaderExt
{
public:
	void Init(GLuint program) override;
	void Use(int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection) override;
};
