﻿#pragma once
#include "Vertex.h"
#include <vector>

namespace rpi
{
	class ModelQuad final
	{
	public:
		enum class Axes
		{
			y, z
		};

		static void Generate(std::vector<Vertex>& vertices, std::vector<int32_t>& indices, 
			Axes secondaryAxes = Axes::z);
	};
}