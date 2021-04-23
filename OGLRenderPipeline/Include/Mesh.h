#pragma once
#include <glad/glad.h>
#include <vector>
#include "Vertex.h"

namespace rpi
{
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<int32_t>& indices, 
			GLuint mode = GL_TRIANGLES);
		virtual ~Mesh();

		virtual void UpdateIbo(const glm::mat4* models, int32_t count);
		virtual void Draw() const;

		virtual bool SwapIbo(GLint ibo = -1);
		
		[[nodiscard]] GLuint GenerateBuffer();

	private:
		GLuint _vao = -1;
		GLuint _ibo = -1;
		GLuint _mode = 0;

		int32_t _size = 0;
		int32_t _count = 0;

		GLuint _currentIbo = -1;

		std::vector<GLuint> _bufferObjects{};
	};
}
