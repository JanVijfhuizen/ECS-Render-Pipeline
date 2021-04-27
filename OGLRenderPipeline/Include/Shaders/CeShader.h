#pragma once
#include <Shader.h>

namespace rpi
{
	// Cecsar class that extends on the shader class.
	class CeShader : public rut::Shader
	{
	public:
		explicit CeShader(GLuint program);

		// Once activated, the following meshes will be drawn using this shader.
		virtual void Use(int32_t index, glm::vec3 eye, 
			const glm::mat4& view, const glm::mat4& projection);
	
	private:
		using Shader::Use;
	};
}
