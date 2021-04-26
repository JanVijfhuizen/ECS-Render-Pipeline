#pragma once
#include "Module.h"
#include "Meshes/Mesh.h"
#include "Components/Renderer.h"
#include "SparseSet.h"
#include "Components/Transform.h"
#include "Shaders/LitShader.h"
#include <memory>
#include "Modules/TextureLoader.h"

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

		Mesh& GetMesh() const;

	private:
		jecs::SparseSet<ExampleFactoryLoader>& _loaders;
		jecs::SparseSet<Renderer>& _renderers;
		jecs::SparseSet<Transform>& _transforms;

		std::unique_ptr<Mesh> _mesh{};
		std::unique_ptr<LitShader> _shader{};

		std::unique_ptr<Texture> _diffuse{};
		std::unique_ptr<Texture> _normal{};
	};
}
