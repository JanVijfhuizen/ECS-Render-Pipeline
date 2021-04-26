#include "Shaders/CeShader.h"

namespace rpi
{
	void CeShader::Use(const int32_t, glm::vec3 eye,
		const glm::mat4& view, const glm::mat4& projection)
	{
		Shader::Use(eye, view, projection);
	}
}