#include "Mesh.h"

#include "Vertex.h"

namespace rut
{
	Mesh::Mesh(const Vertex* vertices, const int32_t vertCount, 
		const int32_t* indices, const int32_t indicesCount)
	{
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Generate VBO.
		const GLuint vbo = GenerateBuffer();
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex),
			vertices, GL_STATIC_DRAW);

		// Generate EBO.
		const GLuint ebo = GenerateBuffer();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint),
			indices, GL_STATIC_DRAW);
		_size = indicesCount;

		// Position attribute.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);

		// Normal attribute.
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);

		// Texture attribute.
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);
	}

	void Mesh::DrawInstanced(const int32_t count)
	{
		glBindVertexArray(_vao);
		glDrawElementsInstanced(mode, _size,
			GL_UNSIGNED_INT, nullptr, count);
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &_vao);
		for (auto& bufferObject : _buffers)
			glDeleteBuffers(1, &bufferObject);
	}

	void Mesh::Draw()
	{
		glBindVertexArray(_vao);
		glDrawElements(mode, _size, GL_UNSIGNED_INT, nullptr);
	}

	GLuint Mesh::GenerateBuffer()
	{
		glBindVertexArray(_vao);

		GLuint buffer;
		glGenBuffers(1, &buffer);
		_buffers.emplace_back(buffer);
		return buffer;
	}

	GLuint Mesh::GetVao() const
	{
		return _vao;
	}

	int32_t Mesh::GetSize() const
	{
		return _size;
	}
}
