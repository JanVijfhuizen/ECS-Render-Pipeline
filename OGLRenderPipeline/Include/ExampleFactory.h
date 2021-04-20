#pragma once
#include "Module.h"
#include "Mesh.h"
#include <memory>
#include "Renderer.h"
#include "SparseSet.h"

namespace rpi::example
{
	class ExampleFactory final : public jecs::Module<ExampleFactory>
	{
	public:
		struct ExampleFactoryLoader final
		{
			
		};

		ExampleFactory();

		void Construct(int32_t index) const;
		void Load() override;

	private:
		jecs::SparseSet<ExampleFactoryLoader>& _loaders;
		jecs::SparseSet<Renderer>& _renderers;

		std::unique_ptr<Mesh> _mesh{};
	};
}
