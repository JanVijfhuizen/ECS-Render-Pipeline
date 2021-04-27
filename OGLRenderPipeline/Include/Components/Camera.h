#pragma once
#include <glm/vec3.hpp>

namespace rpi
{
	// Component used to render the scene with.
	struct Camera final
	{
		// Look at target.
		glm::vec3 target{};
		// Define the up axes for the MVP.
		glm::vec3 up{0, 1, 0};

		// Distance at which to ignore vertices.
		float clipNear = .1f;
		// Distance at which to ignore vertices.
		float clipFar = 1000;

		// Field of View.
		float fov = 45;
		// Models not in these layers will not be rendered with this camera.
		char layer = 0b111111;
	};
}
