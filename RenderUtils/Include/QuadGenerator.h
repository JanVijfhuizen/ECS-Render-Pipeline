#pragma once
#include "Vertex.h"
#include <vector>

namespace rut
{
	// Class used to generate a quad model.
	class QuadGenerator final
	{
	public:
		// Secondary axes used to define a quad.
		enum class Axes
		{
			y, z
		};

		// Generate the model.
		static void Generate(std::vector<Vertex>& vertices, std::vector<int32_t>& indices, 
			Axes secondaryAxes = Axes::z);
	};
}
