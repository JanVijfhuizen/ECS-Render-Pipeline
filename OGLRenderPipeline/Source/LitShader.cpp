#include "LitShader.h"

namespace rpi
{
	LitShader::LitShader(const GLuint memProgram) : Shader(memProgram)
	{
		_color = GetUniformLoc("color");
	}

	void LitShader::Use(const glm::mat4& view, const glm::mat4& projection) const
	{
		Shader::Use(view, projection);

		// Forward textures.
		glBindTexture(GL_TEXTURE_2D, _diffuseTexture);
		glBindTexture(GL_TEXTURE_2D, _normalTexture);

		// Forward color.
		glUniform3f(_color, _colorVal.x, _colorVal.y, _colorVal.z);
	}

	void LitShader::SetColor(const glm::vec3 color)
	{
		_colorVal = color;
	}

	void LitShader::SetDiffuseTex(const GLint handle)
	{
		_diffuseTexture = handle;
	}

	void LitShader::SetNormalTex(const GLint handle)
	{
		_normalTexture = handle;
	}
}
