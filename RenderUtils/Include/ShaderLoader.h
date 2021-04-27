#pragma once
#include <glad/glad.h>
#include <string>

namespace rut
{
	// Used to load in shaders from text files.
	class ShaderLoader final
	{
	public:
		// Create a vertex/fragment/etc shader from a file.
		static GLuint CreateShader(const std::string& filename, GLenum type);

		// Link multiple shaders into one shader program, which can be used to define how models are rendered.
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
