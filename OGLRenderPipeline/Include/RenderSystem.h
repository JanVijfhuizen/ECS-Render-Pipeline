#pragma once
#include <cstdint>
#include "Mesh.h"

namespace rpi
{
	struct Renderer;

	class RenderSystem final
	{
	public:
		static void Update(Mesh::Batch* batches, int32_t batchNum);
	};
}
