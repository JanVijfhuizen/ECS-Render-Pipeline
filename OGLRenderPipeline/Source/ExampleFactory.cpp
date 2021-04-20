#include "ExampleFactory.h"
#include "SparseSet.h"
#include "Renderer.h"

namespace rpi::example
{
	ExampleFactory::ExampleFactory() :
		_loaders(jecs::SparseSet<ExampleFactoryLoader>::Get()),
		_renderers(jecs::SparseSet<Renderer>::Get())	
	{
		std::vector<Vertex> vertices{};
		std::vector<int32_t> indices{};

		_mesh = std::make_unique<Mesh>(vertices, indices);
	}

	void ExampleFactory::Construct(const int32_t index) const
	{
		_loaders.Insert(index);
		auto& renderer = _renderers.Insert(index);
		renderer.mesh = _mesh.get();
	}

	void ExampleFactory::Load()
	{
		for (auto [_, index] : _loaders)
			Construct(index);
	}
}
