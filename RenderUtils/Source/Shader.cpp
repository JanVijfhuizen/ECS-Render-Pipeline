#include "Shader.h"

#include "glm/ext.hpp"

namespace rut
{
	Shader::Shader(const GLuint program) : _program(program)
	{
		glUseProgram(_program);
		_view = GetUniformLoc("view");
		_projection = GetUniformLoc("projection");
	}

	Shader::~Shader()
	{
		glDeleteProgram(_program);
	}

	void Shader::Use(glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection)
	{
		// Use shader program.
		glUseProgram(_program);

		// Forward view.
		glUniformMatrix4fv(_view, 1, GL_FALSE, value_ptr(view));

		// Forward projection.
		glUniformMatrix4fv(_projection, 1, GL_FALSE, value_ptr(projection));
	}

	GLuint Shader::GetProgram() const
	{
		return _program;
	}

	GLuint Shader::GetView() const
	{
		return _view;
	}

	GLuint Shader::GetProjection() const
	{
		return _projection;
	}

	GLuint Shader::GetUniformLoc(const std::string& name) const
	{
		return glGetUniformLocation(_program, name.c_str());
	}
}
