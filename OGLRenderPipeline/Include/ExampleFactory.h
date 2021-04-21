#pragma once
#include "Module.h"
#include "Mesh.h"
#include "Renderer.h"
#include "SparseSet.h"
#include "Transform.h"
#include "LitShader.h"
#include <memory>

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
		void Load() const;

	private:
		jecs::SparseSet<ExampleFactoryLoader>& _loaders;
		jecs::SparseSet<Renderer>& _renderers;
		jecs::SparseSet<Transform>& _transforms;

		std::unique_ptr<Mesh> _mesh{};
		std::unique_ptr<LitShader> _shader{};
	};
}
