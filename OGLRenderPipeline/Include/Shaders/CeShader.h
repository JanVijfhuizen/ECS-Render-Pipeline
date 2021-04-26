#pragma once
#include <Shader.h>

namespace rpi
{
	class CeShader : public rut::Shader
	{
	public:
		explicit CeShader(GLuint program);
		
		virtual void Use(int32_t index, glm::vec3 eye, 
			const glm::mat4& view, const glm::mat4& projection);
	
	private:
		using Shader::Use;
	};
}
