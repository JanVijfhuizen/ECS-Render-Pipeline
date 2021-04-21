#pragma once
#include "Shader.h"

namespace rpi
{
	class LitShader : public Shader
	{
	public:
		explicit LitShader(GLuint memProgram);

		void Use(const glm::mat4& view, const glm::mat4& projection) const override;

		void SetColor(glm::vec3 color = glm::vec3{1});
		void SetDiffuseTex(GLint handle = -1);
		void SetNormalTex(GLint handle = -1);

	private:
		GLuint _color = -1;

		glm::vec3 _colorVal{ 1 };
		GLint _diffuseTexture = -1;
		GLint _normalTexture = -1;
	};
}
