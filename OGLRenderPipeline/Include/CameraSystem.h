#pragma once
#include <glm/mat4x2.hpp>

namespace rpi
{
	class CameraSystem final
	{
	public:
		[[nodiscard]] glm::mat4 static GetMvp(int32_t camIndex, glm::mat4 model = glm::mat4{ 1 });
		[[nodiscard]] glm::mat4 static GetView(int32_t camIndex);
		[[nodiscard]] glm::mat4 static GetProjection(int32_t camIndex);
	};
}
