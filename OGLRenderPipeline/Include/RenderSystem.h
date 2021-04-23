#pragma once
#include <cstdint>

namespace rpi
{
	struct Renderer;

	class RenderSystem final
	{
	public:
		struct Batch final
		{
			int32_t size = 0;
		};

		static void Update(Batch* batches, int32_t batchNum);
	};
}
