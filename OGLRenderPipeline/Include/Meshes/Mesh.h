#pragma once
#include <glad/glad.h>
#include <vector>
#include "glm/ext.hpp"

namespace rpi
{
	class Mesh
	{
		struct Vertex final
		{
			glm::vec3 position{};
			glm::vec3 normal{};
			glm::vec2 texCoords{};
		};
		
	public:
		GLuint mode = GL_TRIANGLES;
		
		Mesh(const Vertex* vertices, int32_t vertCount, 
			const int32_t* indices, int32_t indicesCount);
		virtual ~Mesh();

		virtual void Draw();
		virtual void DrawInstanced(int32_t count) const;

		[[nodiscard]] GLuint GetVao() const;
	
	protected:
		[[nodiscard]] GLuint GenerateBuffer();
		[[nodiscard]] int32_t GetSize() const;
	
	private:
		GLuint _vao = -1;
		int32_t _size = 0;	
		std::vector<GLuint> _buffers{};
	};
}
