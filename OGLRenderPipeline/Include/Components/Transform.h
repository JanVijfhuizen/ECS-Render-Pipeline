#pragma once
#include <glm/vec3.hpp>

namespace rpi
{
	struct Transform final
	{
		glm::vec3 position{0};
		glm::vec3 rotation{0};
		glm::vec3 scale{1};
	};
}
