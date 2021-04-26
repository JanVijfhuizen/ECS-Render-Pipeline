#pragma once
#include "Mesh.h"

namespace rpi
{
	class CeMesh : public rut::Mesh
	{
	public:
		virtual void Draw(int32_t index);
		virtual void DrawInstanced(int32_t index, int32_t count);

	protected:
		using Mesh::Draw;
		using Mesh::DrawInstanced;
	};
}
