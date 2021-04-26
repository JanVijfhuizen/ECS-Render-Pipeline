#include "Meshes/CeMesh.h"

void rpi::CeMesh::Draw(const int32_t)
{
	Mesh::Draw();
}

void rpi::CeMesh::DrawInstanced(const int32_t, const int32_t count)
{
	Mesh::DrawInstanced(count);
}
