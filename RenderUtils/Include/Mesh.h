#pragma once
#include <glad/glad.h>
#include <vector>
#include "glm/ext.hpp"

namespace rut
{
	struct Vertex;

	class Mesh
	{
	public:
		GLuint mode = GL_TRIANGLES;
		
		Mesh(const Vertex* vertices, int32_t vertCount, 
			const int32_t* indices, int32_t indicesCount);
		virtual ~Mesh();

		virtual void Draw();
		virtual void DrawInstanced(int32_t count);

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
