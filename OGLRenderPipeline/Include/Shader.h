#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

namespace rpi
{
	class Shader
	{
	public:
		explicit Shader(GLuint memProgram);
		~Shader();

		virtual void Use(const glm::mat4& view, const glm::mat4& projection);

	protected:
		[[nodiscard]] GLuint GetProgram() const;
		[[nodiscard]] GLuint GetView() const;
		[[nodiscard]] GLuint GetProjection() const;

		GLuint GetUniformLoc(const std::string& name) const;

	private:
		GLuint _program = 0;
		GLuint _view = 0;
		GLuint _projection = 0;
	};
}
