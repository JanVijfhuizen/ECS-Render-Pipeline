#pragma once
#include "Module.h"
#include "Meshes/CeMesh.h"
#include <memory>

#include "Material.h"
#include "TextureLoader.h"
#include "Components/Model.h"
#include "Shaders/CeShader.h"

// Used to streamline entity construction.
class TestFactory final : public jecs::Module<TestFactory>
{
public:
	TestFactory();
	
	// Construct this over an entity.
	void Construct(jecs::Entity entity) const;

private:
	std::unique_ptr<rpi::CeMesh> _mesh{};
	std::unique_ptr<rpi::CeShader> _shader{};

	std::unique_ptr<rut::Texture> _diffuseTex{};
	std::unique_ptr<rut::Texture> _normalTex{};
	std::unique_ptr<rut::Texture> _specularityTex{};
	
	rpi::Model _modelProto{};
	Material _matProto{};
};
