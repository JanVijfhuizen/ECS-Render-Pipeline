#include "InverseEffect.h"
#include <glad/glad.h>
#include "ShaderLoader.h"

namespace rpi
{
	InverseEffect::InverseEffect()
	{
		const GLuint vert = ShaderLoader::CreateShader(
			"Resources/passthrough.vert", GL_VERTEX_SHADER);
		const GLuint frag = ShaderLoader::CreateShader(
			"Resources/inverse.frag", GL_FRAGMENT_SHADER);
		_program = ShaderLoader::LinkShaders(vert, frag);
	}

	InverseEffect::~InverseEffect()
	{
		glDeleteProgram(_program);
	}

	void InverseEffect::Use()
	{
		glUseProgram(_program);
	}
}
