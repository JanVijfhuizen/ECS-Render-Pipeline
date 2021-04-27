#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

namespace rut
{
	// Used to define the way meshes are drawn.
	class Shader
	{
	public:
		explicit Shader(GLuint program);
		virtual ~Shader();

		// Once activated, the following meshes will be drawn using this shader.
		virtual void Use(glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection);

		// Get the shader program which this shader uses.
		[[nodiscard]] GLuint GetProgram() const;
		// Get target uniform location in the shader program.
		[[nodiscard]] GLuint GetUniformLoc(const std::string& name) const;
	
	protected:
		// Get the camera view handle.
		[[nodiscard]] GLuint GetView() const;
		// Get the camera projection handle.
		[[nodiscard]] GLuint GetProjection() const;

	private:
		GLuint _program = 0;
		GLuint _view = 0;
		GLuint _projection = 0;
	};
}
