#include "InverseEffect.h"

#include <glad/glad.h>
#include "ShaderLoader.h"

namespace rpi
{
	InverseEffect::InverseEffect()
	{
		const GLuint vert = rut::ShaderLoader::CreateShader(
			"Resources/passthrough.vert", GL_VERTEX_SHADER);
		const GLuint frag = rut::ShaderLoader::CreateShader(
			"Resources/inverse.frag", GL_FRAGMENT_SHADER);
		_program = rut::ShaderLoader::LinkShaders(vert, frag);
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
