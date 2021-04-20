#include "ModelQuad.h"
namespace rpi 
{
	void ModelQuad::Generate(std::vector<Vertex>& vertices, std::vector<int32_t>& indices)
	{
		vertices.emplace_back(Vertex{
			{ .5f, 0, .5f },
			{ 0, 0, 1 },
			{ 1, 1 } });

		vertices.emplace_back(Vertex{
			{ .5f, 0, -.5f },
			{ 0, 0, 1 },
			{ 1, 0 } });

		vertices.emplace_back(Vertex{
			{ -.5f, 0, -.5f },
			{ 0, 0, 1 },
			{ 0, 0 } });

		vertices.emplace_back(Vertex{
			{ -.5f, 0, .5f },
			{ 0, 0, 1 },
			{ 0, 1 } });

		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(3);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(3);
	}
}