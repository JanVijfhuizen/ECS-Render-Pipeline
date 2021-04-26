#include "TestFactory.h"

#include "BasicShader.h"
#include "QuadGenerator.h"
#include "ShaderLoader.h"
#include "SparseSet.h"
#include "Vertex.h"
#include "Components/Transform.h"

TestFactory::TestFactory()
{
	// Define shader.
	const GLuint frag = rut::ShaderLoader::CreateShader(
		"Resources/fragLit.frag", GL_FRAGMENT_SHADER);
	const GLuint vert = rut::ShaderLoader::CreateShader(
		"Resources/vert.vert", GL_VERTEX_SHADER);
	const GLuint linked = rut::ShaderLoader::LinkShaders(frag, vert);
	_shader = std::make_unique<BasicShader>(linked);
	
	// Define mesh.
	std::vector<rut::Vertex> vertices;
	std::vector<int32_t> indices;

	rut::QuadGenerator::Generate(vertices, indices, rut::QuadGenerator::Axes::y);
	_mesh = std::make_unique<rpi::CeMesh>(
		vertices.data(), vertices.size(), 
		indices.data(), indices.size());

	_modelProto.shader = _shader.get();
	_modelProto.mesh = _mesh.get();
}

void TestFactory::Construct(const jecs::Entity entity) const
{
	const auto& index = entity.index;
	jecs::SparseSet<rpi::Model>::Get().Insert(index, _modelProto);
	jecs::SparseSet<rpi::Transform>::Get().Insert(index);
}
