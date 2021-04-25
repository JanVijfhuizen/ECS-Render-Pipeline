#pragma once
#include "Mesh.h"

namespace rpi
{
	class Mesh_im4 : public Mesh
	{
	public:
		Mesh_im4(std::vector<Vertex>& vertices, std::vector<int32_t>& indices, GLuint mode = GL_TRIANGLES);

	private:
		void OnFillBatch(const int32_t* indexes, int32_t size) override;
		void DefineIbo() override;
	};
}
