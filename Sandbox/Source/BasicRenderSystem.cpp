#include "BasicRenderSystem.h"

#include "MapSet.h"
#include "SparseSet.h"
#include "Components/Camera.h"
#include "Components/Model.h"
#include "Components/Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Systems/CameraSystem.h"

void BasicRenderSystem::Update()
{
	auto& cameras = jecs::MapSet<rpi::Camera>::Get();
	auto& models = jecs::SparseSet<rpi::Model>::Get();
	const auto& transforms = jecs::SparseSet<rpi::Transform>::Get();

	for (const auto [camera, camIndex] : cameras)
	{
		const auto eye = transforms[camIndex].position;
		const auto view = rpi::CameraSystem::GetView(camIndex);
		const auto projection = rpi::CameraSystem::GetProjection(camIndex);

		for (const auto [model, modIndex] : models)
		{
			if (rpi::CameraSystem::Ignore(camera, model.layer))
				continue;

			model.shader->Use(eye, view, projection);
			model.mesh->Draw();
		}
	}
}
