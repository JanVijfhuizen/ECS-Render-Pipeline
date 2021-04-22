#include "ModelQuad.h"
namespace rpi 
{
	void ModelQuad::Generate(std::vector<Vertex>& vertices, std::vector<int32_t>& indices,
		Axes secondaryAxes)
	{
		const int32_t yMul = 1 - static_cast<int32_t>(secondaryAxes);
		const int32_t zMul = static_cast<int32_t>(secondaryAxes);

		vertices.emplace_back(Vertex{
			{ 1, yMul, zMul },
			{ 0, 1, 0 },
			{ 1, 1 } });

		vertices.emplace_back(Vertex{
			{ 1, -yMul, -zMul },
			{ 0, 1, 0 },
			{ 1, 0 } });

		vertices.emplace_back(Vertex{
			{ -1, -yMul, -zMul },
			{ 0, 1, 0 },
			{ 0, 0 } });

		vertices.emplace_back(Vertex{
			{ -1, yMul, zMul },
			{ 0, 1, 0 },
			{ 0, 1 } });

		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(3);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(3);
	}
}