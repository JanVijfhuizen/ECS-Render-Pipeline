#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

namespace rut
{
	class Shader
	{
	public:
		explicit Shader(GLuint program);
		virtual ~Shader();

		virtual void Use(glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection);

		[[nodiscard]] GLuint GetProgram() const;
		[[nodiscard]] GLuint GetUniformLoc(const std::string& name) const;
	
	protected:
		[[nodiscard]] GLuint GetView() const;
		[[nodiscard]] GLuint GetProjection() const;

	private:
		GLuint _program = 0;
		GLuint _view = 0;
		GLuint _projection = 0;
	};
}
