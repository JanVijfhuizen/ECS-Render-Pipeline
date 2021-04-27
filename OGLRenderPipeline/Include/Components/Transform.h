#pragma once
#include <glm/vec3.hpp>

namespace rpi
{
	// Component used to define the position, rotation and scale in the world.
	struct Transform final
	{
		glm::vec3 position{0};
		glm::vec3 rotation{0};
		glm::vec3 scale{1};
	};
}
