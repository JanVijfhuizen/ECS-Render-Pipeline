#include "ShaderLoader.h"

#include <fstream>
#include <vector>

namespace rut
{
	GLuint ShaderLoader::CreateShader(const std::string& filename, const GLenum type)
	{
		std::ifstream file(filename, std::ios::ate);

		if (!file.is_open())
		{
			fprintf(stdin, "Couldn't load shader: %s\n", filename.c_str());
			return 0;
		}

		// Read contents from file and paste it as a shader.
		const size_t size = static_cast<size_t>(file.tellg());
		std::vector<char> shaderSource(size);
		file.seekg(0);
		file.read(shaderSource.data(), size);
		shaderSource.push_back(0);

		const auto shader = glCreateShader(type);
		auto* data = shaderSource.data();

		glShaderSource(shader, 1, &data, nullptr);
		glCompileShader(shader);

		// Assert if the shader has been loaded successfully.
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char log[_BUFFER];
			glGetShaderInfoLog(shader,
				_BUFFER, nullptr, log);
			printf("Shader not compiled: %s\n", log);

			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}
}