#include "Meshes/CeMesh.h"

rpi::CeMesh::CeMesh(const rut::Vertex* vertices, const int32_t vertCount, 
	const int32_t* indices, const int32_t indicesCount) :
	Mesh(vertices, vertCount, indices, indicesCount)
{
	
}

void rpi::CeMesh::Draw(const int32_t)
{
	Mesh::Draw();
}

void rpi::CeMesh::DrawInstanced(const int32_t, const int32_t count)
{
	Mesh::DrawInstanced(count);
}
