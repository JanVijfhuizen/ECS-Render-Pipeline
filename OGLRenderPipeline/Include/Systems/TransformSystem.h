#pragma once
#include <glm/mat4x2.hpp>

namespace rpi
{
	struct Transform;

	class TransformSystem final
	{
	public:
		static glm::mat4 GetMatrix(const Transform& transform);
		static glm::vec3 GetForward(const Transform& transform);
	};
}
