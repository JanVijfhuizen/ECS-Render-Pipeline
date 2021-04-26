#pragma once
#include <Shader.h>

namespace rpi
{
	class CeShader : public rut::Shader
	{
	public:
		virtual void Use(int32_t index, glm::vec3 eye, 
			const glm::mat4& view, const glm::mat4& projection);
	
	protected:
		using Shader::Use;
	};
}
