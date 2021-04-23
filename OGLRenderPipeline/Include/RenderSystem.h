#pragma once
#include <cstdint>
#include <glad/glad.h>

namespace rpi
{
	struct Renderer;

	class RenderSystem final
	{
	public:
		struct Batch final
		{
			int32_t size = 0;
			GLint ibo = -1;
		};

		static void Update(Batch* batches, int32_t batchNum);
	};
}
