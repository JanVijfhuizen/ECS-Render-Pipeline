#include "TestFactory.h"

#include "LightFwd.h"
#include "QuadGenerator.h"
#include "ShaderExtensions.h"
#include "ShaderLoader.h"
#include "SparseSet.h"
#include "TextureLoader.h"
#include "Vertex.h"
#include "Components/Transform.h"
#include "../ObjLoader.h"

TestFactory::TestFactory()
{
	// Define shader.
	const GLuint frag = rut::ShaderLoader::CreateShader(
		"Resources/lit.frag", GL_FRAGMENT_SHADER);
	const GLuint vert = rut::ShaderLoader::CreateShader(
		"Resources/lit.vert", GL_VERTEX_SHADER);
	const GLuint linked = rut::ShaderLoader::LinkShaders(frag, vert);
	_shader = std::make_unique<rpi::ModularShader<ModelFwd, MatFwd, LightmapFwd, LightFwd>>(linked);
	
	// Define mesh.
	std::vector<rut::Vertex> vertices;
	std::vector<int32_t> indices;
	ObjLoader::CreateModel("Resources/Gun.obj", vertices, indices);
	// Generate quad mesh.
	//rut::QuadGenerator::Generate(vertices, indices, rut::QuadGenerator::Axes::y);
	_mesh = std::make_unique<rpi::CeMesh>(vertices.data(), vertices.size(), 
		indices.data(), indices.size());

	// Set up prototype component.
	// This will be copied into the set for newly constructed entities.
	_modelProto.shader = _shader.get();
	_modelProto.mesh = _mesh.get();
	
	// Define material.
	_diffuseTex = rut::TextureLoader::Load("Resources/tex.png");
	_normalTex = rut::TextureLoader::Load("Resources/normal.png");
	// Just using a normal texture as a specular map.
	_specularityTex = rut::TextureLoader::Load("Resources/tex.png");

	// Set up prototype component.
	_matProto.diffuseTex = _diffuseTex->GetHandle();
	_matProto.normalTex = _normalTex->GetHandle();
	_matProto.specularityTex = _specularityTex->GetHandle();
}

void TestFactory::Construct(const jecs::Entity entity) const
{
	// Define and add the required components.
	const auto& index = entity.index;
	jecs::SparseSet<rpi::Model>::Get().Insert(index, _modelProto);
	jecs::SparseSet<Material>::Get().Insert(index, _matProto);
	jecs::SparseSet<rpi::Transform>::Get().Insert(index);
}
