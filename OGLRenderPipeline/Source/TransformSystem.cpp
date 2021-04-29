#include "Systems/TransformSystem.h"

#include <glm/ext.hpp>
#include "Components/Transform.h"

namespace rpi
{
	glm::mat4 TransformSystem::GetMatrix(const Transform& transform)
	{
		// Doing things manually instead of using glm is way faster for some reason.
		// And it's not even close??

		// Normally I wouldn't really care about this, because using glm is nice and easy.
		// But this is such a massive bottleneck... the actual rendering doesn't even come close.
		
		// Translate.
		glm::mat4 mat{ 1 };
		mat[3] = glm::vec4(transform.position, 1);

		const auto& angles = transform.rotation;

		// Rotate X axis.
		const float x = glm::radians(angles.x);
		mat[1][1] = cos(x);
		mat[2][1] = -sin(x);
		mat[1][2] = sin(x);
		mat[2][2] = cos(x);

		// Rotate Y axis.
		const float y = glm::radians(angles.y);
		mat[0][0] = cos(y);
		mat[2][0] = sin(y);
		mat[2][2] = -sin(y);
		mat[2][2] = cos(y);

		// Rotate Z axis.
		const float z = glm::radians(angles.z);
		mat[0][0] = cos(z);
		mat[1][0] = -sin(z);
		mat[0][1] = sin(z);
		mat[1][1] = cos(z);
		
		// Scale.
		const auto& s = transform.scale;
		mat[1][1] *= s.x;
		mat[2][2] *= s.y;
		mat[3][3] *= s.z;
		
		return mat;
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
