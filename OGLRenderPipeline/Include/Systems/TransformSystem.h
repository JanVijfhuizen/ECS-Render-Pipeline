#pragma once
#include <glm/mat4x2.hpp>

namespace rpi
{
	struct Transform;

	// System that contains useful transform methods. 
	class TransformSystem final
	{
	public:
		// Convert the transform into a 4x4 matrix, used for rendering.
		// Note that this is extremely expensive.
		static glm::mat4 GetMatrix(const Transform& transform);
		// Get the forward axes of a transform.
		static glm::vec3 GetForward(const Transform& transform);
	};
}
