#pragma once
#include <glm/vec3.hpp>

namespace rpi
{
	struct Camera final
	{
		glm::vec3 target{};
		glm::vec3 up{0, 1, 0};

		float clipNear = .1f;
		float clipFar = 1000;

		float fov = 45;
		char layer = 0b111111;
	};
}
