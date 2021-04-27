#include "Systems/TransformSystem.h"

#include <glm/ext.hpp>
#include "Components/Transform.h"

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

	glm::vec3 TransformSystem::GetForward(const Transform& transform)
	{
		// Set direction.
		const auto& rotation = transform.rotation;
		const glm::vec3 rad = radians(rotation);

		const float xPos = sin(rad.y) * cos(rad.x);
		const float yPos = sin(rad.x);
		const float zPos = cos(rad.x) * cos(rad.y);

		return { xPos, yPos, zPos };
	}
}
