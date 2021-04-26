#include "Shaders/Shader.h"

#include "glm/ext.hpp"

namespace rpi
{
	Shader::Shader(const GLuint memProgram) : _program(memProgram)
	{
		glUseProgram(_program);
		_view = GetUniformLoc("view");
		_projection = GetUniformLoc("projection");
	}

	Shader::~Shader()
	{
		glDeleteProgram(_program);
	}

	void Shader::Use(glm::vec3 camPos, const glm::mat4& view, const glm::mat4& projection)
	{
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
		const GLint loc = glGetUniformLocation(_program, name.c_str());
		assert(loc != -1);
		return loc;
	}
}
