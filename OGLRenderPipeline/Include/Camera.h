#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include "PostEffect.h"

namespace rpi
{
	class PostEffect;

	struct Camera final
	{
		bool enabled = true;

		glm::vec3 target{};
		glm::vec3 up{ 0, 1, 0 };

		float fov = 45;

		float clipNear = .1f;
		float clipFar = 1000;

		char layer = 0b111111;

		std::vector<PostEffect*> postProcStack{};
	};
}
