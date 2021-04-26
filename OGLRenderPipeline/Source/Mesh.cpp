#include "Meshes/Mesh.h"

#include "SparseSet.h"

namespace rpi
{
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<int32_t>& indices, const GLuint mode) :
		_mode(mode)
	{
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Generate VBO.
		const GLuint vbo = GenerateBuffer();
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
			vertices.data(), GL_STATIC_DRAW);

		// Generate EBO.
		const GLuint ebo = GenerateBuffer();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
			indices.data(), GL_STATIC_DRAW);
		_size = indices.size();

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
		
		// Generate IBO and bind it.
		_ibo = GenerateBuffer();
		glBindBuffer(GL_ARRAY_BUFFER, _ibo);
		_currentIbo = _ibo;
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &_vao);
		for (auto& bufferObject : _bufferObjects)
			glDeleteBuffers(1, &bufferObject);
	}

	void Mesh::SwapBatch(const Batch batch)
	{
		glBindVertexArray(_vao);
		const GLuint targetIbo = batch.ibo == -1 ? _ibo : batch.ibo;

		_count = batch.size;
		_currentIbo = targetIbo;
	}

	void Mesh::FillBatch(const int32_t* indexes, const int32_t size)
	{
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _ibo);		
		OnFillBatch(indexes, size);
	}

	void Mesh::Draw() const
	{
		glBindVertexArray(_vao);
		glDrawElementsInstanced(_mode, _size,
			GL_UNSIGNED_INT, 0, _count);
	}

	GLuint Mesh::GenerateBuffer()
	{
		glBindVertexArray(_vao);

		GLuint buffer;
		glGenBuffers(1, &buffer);
		_bufferObjects.emplace_back(buffer);
		return buffer;
	}
}
