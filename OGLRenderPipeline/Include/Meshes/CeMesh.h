#pragma once
#include "Mesh.h"

namespace rpi
{
	// Cecsar class that extends on the mesh class.
	class CeMesh : public rut::Mesh
	{
	public:
		CeMesh(const rut::Vertex* vertices, int32_t vertCount,
			const int32_t* indices, int32_t indicesCount);

		// Draw the mesh on the current frame buffer.
		virtual void Draw(int32_t index);
		// Draw the meshes instanced. Take note that instancing is handled outside of the mesh class.
		virtual void DrawInstanced(int32_t index, int32_t count);

	private:
		using Mesh::Draw;
		using Mesh::DrawInstanced;
	};
}
