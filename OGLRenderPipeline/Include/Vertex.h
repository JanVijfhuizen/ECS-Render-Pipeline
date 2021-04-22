#pragma once
#include "glm/ext.hpp"

namespace rpi
{
	struct Vertex final
	{
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec2 texCoords{};
	};
}