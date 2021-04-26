#pragma once
#include "Mesh.h"

namespace rpi
{
	class CeMesh : public rut::Mesh
	{
	public:
		CeMesh(const rut::Vertex* vertices, int32_t vertCount,
			const int32_t* indices, int32_t indicesCount);
		
		virtual void Draw(int32_t index);
		virtual void DrawInstanced(int32_t index, int32_t count);

	private:
		using Mesh::Draw;
		using Mesh::DrawInstanced;
	};
}
