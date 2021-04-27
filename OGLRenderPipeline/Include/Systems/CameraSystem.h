#pragma once
#include <glm/mat4x2.hpp>

namespace rpi
{
	struct Camera;

	// System that contains useful camera methods.
	class CameraSystem final
	{
	public:
		// Get the Model View Projection, used to define a mesh it's place on the screen.
		[[nodiscard]] glm::mat4 static GetMvp(int32_t camIndex, glm::mat4 model = glm::mat4{ 1 });
		// Get the camera view, used to define the MVP.
		[[nodiscard]] glm::mat4 static GetView(int32_t camIndex);
		// Get the camera projection, used to define the MVP.
		[[nodiscard]] glm::mat4 static GetProjection(int32_t camIndex);
		
		// Checks if this camera ignores target layer.
		static bool Ignore(Camera& camera, char layer);
	};
}
