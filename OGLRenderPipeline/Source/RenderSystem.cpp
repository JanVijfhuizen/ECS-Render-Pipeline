#include "RenderSystem.h"
#include "Renderer.h"
#include "SparseSet.h"

namespace rpi
{
	void RenderSystem::Update()
	{
		auto& renderers = jecs::SparseSet<Renderer>::Get();
		for (auto [renderer, index] : renderers)
		{
			// Do the thing.
		}
	}
}
