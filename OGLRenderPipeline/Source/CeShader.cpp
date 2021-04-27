#include "Shaders/CeShader.h"

namespace rpi
{
	CeShader::CeShader(const GLuint program) : Shader(program)
	{
		
	}

	void CeShader::Use(const int32_t, const glm::vec3 eye,
	                   const glm::mat4& view, const glm::mat4& projection)
	{
		Shader::Use(eye, view, projection);
	}
}