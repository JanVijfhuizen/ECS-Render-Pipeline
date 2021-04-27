#pragma once
#include "Module.h"
#include "Meshes/CeMesh.h"
#include <memory>

#include "Components/Model.h"
#include "Shaders/CeShader.h"

class TestFactory final : public jecs::Module<TestFactory>
{
public:
	TestFactory();
	void Construct(jecs::Entity entity) const;

private:
	std::unique_ptr<rpi::CeMesh> _mesh{};
	std::unique_ptr<rpi::CeShader> _shader{};

	rpi::Model _modelProto{};
};
