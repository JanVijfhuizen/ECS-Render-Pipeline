#include "TransformSystem.h"
#include <glm/ext.hpp>
#include "Transform.h"

namespace rpi
{
	glm::mat4 TransformSystem::GetMatrix(const Transform& transform)
	{
		glm::mat4 mat{ 1 };
		mat = translate(mat, transform.position);

		const auto& angles = transform.rotation;
		const auto euler = glm::eulerAngleXYZ(
			glm::radians(angles.x) / 2,
			glm::radians(angles.y) / 2,
			glm::radians(angles.z) / 2);
		mat = mat * euler;

		return scale(mat, transform.scale);
	}
}