#include "Mesh_im4.h"
#include "SparseSet.h"
#include "TransformSystem.h"
#include "Transform.h"

namespace rpi
{
	Mesh_im4::Mesh_im4(std::vector<Vertex>& vertices, std::vector<int32_t>& indices, const GLuint mode):
		Mesh(vertices, indices, mode)
	{
		Mesh_im4::DefineIbo();
	}

	void Mesh_im4::OnFillBatch(const int32_t* indexes, const int32_t size)
	{
		const auto& transforms = jecs::SparseSet<Transform>::Get();

		// Avoid (de)allocating on the heap with single objects.
		if (size == 1)
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

	void Mesh_im4::DefineIbo()
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
