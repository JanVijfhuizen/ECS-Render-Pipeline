#pragma once
#include <glad/glad.h>
#include <string>

namespace rut
{
	class ShaderLoader final
	{
	public:
		static GLuint CreateShader(const std::string& filename, GLenum type);

		template <typename... Args>
		static GLuint LinkShaders(Args ... shaders);

	private:
#define _BUFFER 1000
	};

	template <typename ... Args>
	GLuint ShaderLoader::LinkShaders(Args... shaders)
	{
		auto program = glCreateProgram();
		(glAttachShader(program, shaders), ...);

		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char log[_BUFFER];
			glGetProgramInfoLog(program,
				_BUFFER, nullptr, log);
			printf("Shader linking failed: %s\n", log);

			glDeleteProgram(program);
			return 0;
		}

		(glDeleteShader(shaders), ...);
		return program;
	}
}
