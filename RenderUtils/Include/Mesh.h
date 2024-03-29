#pragma once
#include <glad/glad.h>
#include <vector>

namespace rut
{
	struct Vertex;

	// Used to define the geometry of a model.
	class Mesh
	{
	public:
		// Draws triangles by default, but can also be changed to draw lines, or points.
		GLuint mode = GL_TRIANGLES;

		// Only sets up the vao. Useful for when you want to use user defined vertices.
		Mesh();
		// Constructor that takes in a standard vertex.
		Mesh(const Vertex* vertices, int32_t vertCount, 
			const int32_t* indices, int32_t indicesCount);
		virtual ~Mesh();

		// Draw the mesh on the current frame buffer.
		virtual void Draw();
		// Draw the meshes instanced. Take note that instancing is handled outside of the mesh class.
		virtual void DrawInstanced(int32_t count);

		// Get the Vertex Array Buffer which this mesh uses.
		[[nodiscard]] GLuint GetVao() const;
	
	protected:
		int32_t _size = 0;
		
		// Generate a buffer. These buffers will be deallocated once the mesh gets destroyed.
		[[nodiscard]] GLuint GenerateBuffer();
	
	private:
		GLuint _vao = -1;
		std::vector<GLuint> _buffers{};
	};
}
