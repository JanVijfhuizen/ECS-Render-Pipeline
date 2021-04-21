#include "ExampleFactory.h"
#include "SparseSet.h"
#include "Renderer.h"
#include "ModelQuad.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"

namespace rpi::example
{
	ExampleFactory::ExampleFactory() :
		_loaders(jecs::SparseSet<ExampleFactoryLoader>::Get()),
		_renderers(jecs::SparseSet<Renderer>::Get()),
		_transforms(jecs::SparseSet<Transform>::Get())
	{
		// Define mesh.
		std::vector<Vertex> vertices{};
		std::vector<int32_t> indices{};
		ModelQuad::Generate(vertices, indices);
		_mesh = std::make_unique<Mesh>(vertices, indices);

		// Define model.
		const GLuint frag = ShaderLoader::CreateShader(
			"Resources/frag.frag", GL_FRAGMENT_SHADER);
		const GLuint vert = ShaderLoader::CreateShader(
			"Resources/vert.vert", GL_VERTEX_SHADER);
		const GLuint linked = ShaderLoader::LinkShaders(frag, vert);
		_shader = std::make_unique<LitShader>(linked);

		_shader->SetDiffuseTex(TextureLoader::Load("Resources/texture.png"));
		_shader->SetNormalTex(TextureLoader::Load("Resources/texture.png"));
	}

	void ExampleFactory::Construct(const int32_t index) const
	{
		_loaders.Insert(index);
		_transforms.Insert(index);

		auto& renderer = _renderers.Insert(index);
		renderer.mesh = _mesh.get();
		renderer.shader = _shader.get();
	}

	void ExampleFactory::Load() const
	{
		for (const auto [_, index] : _loaders)
			Construct(index);
	}
}
