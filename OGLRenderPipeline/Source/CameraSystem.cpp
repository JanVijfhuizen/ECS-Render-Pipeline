#include "Systems/CameraSystem.h"

#include "MapSet.h"
#include "glm/ext.hpp"
#include "SparseSet.h"
#include "Components/Camera.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Modules/WindowModule.h"

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

	bool CameraSystem::Ignore(Camera& camera, Renderer& renderer)
	{
		return (camera.layer & renderer.layer) == 0;
	}
}
