#include "Mesh.h"


#include "SparseSet.h"
#include "Transform.h"
#include "TransformSystem.h"

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

		// Define the default mat4 shaped ibo.
		Mesh::DefineIbo();
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

	void Mesh::OnFillBatch(const int32_t* indexes, const int32_t size)
	{
		const auto& transforms = jecs::SparseSet<Transform>::Get();

		// Avoid (de)allocating on the heap with single objects.
		if(size == 1)
		{
			const auto& transform = transforms[indexes[0]];
			const auto mat = TransformSystem::GetMatrix(transform);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4),
				&mat, GL_DYNAMIC_DRAW);
			return;
		}

		auto* const mats = new glm::mat4[size];
		for (int32_t i = 0; i < size; ++i)
		{
			const auto& transform = transforms[indexes[i]];
			mats[i] = TransformSystem::GetMatrix(transform);
		}

		glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::mat4),
			mats, GL_DYNAMIC_DRAW);
		delete[] mats;
	}

	void Mesh::DefineIbo()
	{
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
}
