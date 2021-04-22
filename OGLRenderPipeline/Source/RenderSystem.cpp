#include "RenderSystem.h"
#include "Renderer.h"
#include "SparseSet.h"
#include "MapSet.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "PostProcessingModule.h"

namespace rpi
{
	void RenderSystem::Update()
	{
		auto& cameras = jecs::MapSet<Camera>::Get();
		auto& renderers = jecs::SparseSet<Renderer>::Get();
		auto& transforms = jecs::SparseSet<Transform>::Get();

		auto& postProcModule = PostProcessingModule::Get();

		for (const auto [camera, camIndex] : cameras)
		{
			postProcModule.RenderNext();

			const auto& camPos = transforms[camIndex].position;
			const auto projection = CameraSystem::GetProjection(camIndex);
			const auto view = CameraSystem::GetView(camIndex);

			for (const auto [renderer, renIndex] : renderers)
			{
				const auto& transform = transforms[renIndex];
				const auto model = TransformSystem::GetMatrix(transform);

				assert(renderer.mesh);
				assert(renderer.shader);
				
				renderer.shader->Use(camPos, view, projection);
				renderer.mesh->UpdateInstanceBuffer(&model, 1);
				renderer.mesh->Draw();
			}

			postProcModule.RenderPostEffects(camera.postProcStack);
		}

		postProcModule.PostRender();
	}
}
