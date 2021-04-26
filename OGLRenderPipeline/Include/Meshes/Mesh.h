#pragma once
#include <glad/glad.h>
#include <vector>
#include "Vertex.h"

namespace rpi
{
	class Mesh
	{
	public:
		struct Batch final
		{
			int32_t size = 1;
			GLint ibo = -1;
		};
		
		Mesh(std::vector<Vertex>& vertices, std::vector<int32_t>& indices, 
			GLuint mode = GL_TRIANGLES);
		virtual ~Mesh();

		void SwapBatch(Batch batch = {});
		void FillBatch(const int32_t* indexes, int32_t size);
		
		void Draw() const;
		
		[[nodiscard]] GLuint GenerateBuffer();

	protected:
		virtual void OnFillBatch(const int32_t* indexes, int32_t size) = 0;
		virtual void DefineIbo() = 0;

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
