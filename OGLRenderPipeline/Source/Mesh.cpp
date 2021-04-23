#include "Mesh.h"

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

		const std::size_t vec4Size = sizeof(glm::vec4);
		const std::size_t mat4Size = vec4Size * 4;
		const int32_t iboBegin = 3;

		glVertexAttribPointer(iboBegin, 4, GL_FLOAT,
			GL_FALSE, mat4Size, nullptr);

		glVertexAttribPointer(iboBegin + 1, 4, GL_FLOAT,
			GL_FALSE, mat4Size, (void*)(1 * vec4Size));

		glVertexAttribPointer(iboBegin + 2, 4, GL_FLOAT,
			GL_FALSE, mat4Size, (void*)(2 * vec4Size));

		glVertexAttribPointer(iboBegin + 3, 4, GL_FLOAT,
			GL_FALSE, mat4Size, (void*)(3 * vec4Size));

		// Enable IBO attributes.
		glEnableVertexAttribArray(iboBegin);
		glEnableVertexAttribArray(iboBegin + 1);
		glEnableVertexAttribArray(iboBegin + 2);
		glEnableVertexAttribArray(iboBegin + 3);

		glVertexAttribDivisor(iboBegin, 1);
		glVertexAttribDivisor(iboBegin + 1, 1);
		glVertexAttribDivisor(iboBegin + 2, 1);
		glVertexAttribDivisor(iboBegin + 3, 1);
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &_vao);
		for (auto& bufferObject : _bufferObjects)
			glDeleteBuffers(1, &bufferObject);
	}

	void Mesh::UpdateIbo(const glm::mat4* models, const int32_t count)
	{
		_count = count;
		glBindBuffer(GL_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4),
			models, GL_DYNAMIC_DRAW);
	}

	void Mesh::Draw() const
	{
		glBindVertexArray(_vao);
		glDrawElementsInstanced(_mode, _size,
			GL_UNSIGNED_INT, 0, _count);
	}

	bool Mesh::SwapIbo(const GLint ibo)
	{
		const GLuint targetIbo = ibo == -1 ? _ibo : ibo;
		if (_currentIbo == targetIbo)
			return false;
		_currentIbo = targetIbo;

		const std::size_t vec4Size = sizeof(glm::vec4);
		const std::size_t mat4Size = vec4Size * 4;
		const int32_t iboBegin = 3;
		
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, targetIbo);

		glVertexAttribPointer(iboBegin, 4, GL_FLOAT,
			GL_FALSE, mat4Size, nullptr);

		glVertexAttribPointer(iboBegin + 1, 4, GL_FLOAT,
			GL_FALSE, mat4Size, (void*)(1 * vec4Size));

		glVertexAttribPointer(iboBegin + 2, 4, GL_FLOAT,
			GL_FALSE, mat4Size, (void*)(2 * vec4Size));

		glVertexAttribPointer(iboBegin + 3, 4, GL_FLOAT,
			GL_FALSE, mat4Size, (void*)(3 * vec4Size));
		return true;
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
