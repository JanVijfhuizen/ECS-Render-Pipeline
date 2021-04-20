#include "CameraSystem.h"
#include "Camera.h"
#include "MapSet.h"
#include "glm/ext.hpp"
#include "WindowModule.h"
#include "SparseSet.h"
#include "Transform.h"

namespace rpi
{
	glm::mat4 CameraSystem::GetMvp(const int32_t camIndex, const glm::mat4 model)
	{
		const auto view = GetView(camIndex);
		const auto projection = GetProjection(camIndex);

		return projection * view * model;
	}

	glm::mat4 CameraSystem::GetView(const int32_t camIndex)
	{
		const auto& camera = jecs::MapSet<Camera>::Get()[camIndex];
		const auto& transform = jecs::SparseSet<Transform>::Get()[camIndex];

		return lookAt(transform.position, camera.target,
			camera.up);
	}

	glm::mat4 CameraSystem::GetProjection(const int32_t camIndex)
	{
		const auto& camera = jecs::MapSet<Camera>::Get()[camIndex];
		const auto& windowModule = WindowModule::Get();
		const auto& windowSettings = windowModule.GetSettings();;

		return glm::perspective(glm::radians(camera.fov),
			windowSettings.GetAspectRatio(), camera.clipNear, camera.clipFar);
	}
}
