#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include "PostProcessingEffect.h"

namespace rpi
{
	class PostProcessingEffect;

	struct Camera final
	{
		bool enabled = true;

		glm::vec3 target{};
		glm::vec3 up{ 0, 1, 0 };

		float fov = 45;

		float clipNear = .1f;
		float clipFar = 1000;

		std::vector<PostProcessingEffect*> postProcStack{};
	};
}
