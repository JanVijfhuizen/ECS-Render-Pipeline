#include "Systems/RenderSystem.h"

#include "SparseSet.h"
#include "MapSet.h"
#include "Components/Camera.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Meshes/Mesh.h"
#include "Modules/PostProcessingModule.h"
#include "Systems/CameraSystem.h"

namespace rpi
{
	void RenderSystem::Update(Mesh::Batch* batches, const int32_t batchNum)
	{
		auto& cameras = jecs::MapSet<Camera>::Get();
		auto& renderers = jecs::SparseSet<Renderer>::Get();
		auto& transforms = jecs::SparseSet<Transform>::Get();
		
		const auto& renderValues = renderers.GetValuesRaw();
		const auto& renderDense = renderers.GetDenseRaw();

		const int32_t renderCount = renderers.GetCount();
		int32_t batchEnd = 0;
		for (int32_t i = batchNum - 1; i >= 0; --i)
			batchEnd += batches[i].size;
		
		auto& postProcModule = PostProcessingModule::Get();

		// Draw every camera's viewpoint.
		for (const auto [camera, camIndex] : cameras)
		{
			if (!camera.enabled)
				continue;
			
			postProcModule.RenderBegin(camera.postProcStack);

			const auto& camPos = transforms[camIndex].position;
			const auto projection = CameraSystem::GetProjection(camIndex);
			const auto view = CameraSystem::GetView(camIndex);

			// Render batches.
			int32_t currentBatch = 0;
			for (int32_t i = 0; i < batchEnd;)
			{
				auto& renderer = renderValues[i];
				const auto& batch = batches[currentBatch++];
				
				// If the batch isn't culled or ignored.
				if (!renderer.isCulled && !CameraSystem::Ignore(camera, renderer))
				{
					renderer.shader->Use(camPos, view, projection);
					
					renderer.mesh->SwapBatch(batch);
					renderer.mesh->Draw();
				}

				i += batch.size;
			}

			// Render non batched instances.
			for (int32_t i = batchEnd; i < renderCount; ++i)
			{
				auto& renderer = renderers[i];

				// Culling can be done by an external system.
				if (renderer.isCulled)
					continue;

				// If the camera doesn't render this layer.
				if (CameraSystem::Ignore(camera, renderer))
					continue;
				
				// Use shader and render model.
				renderer.shader->Use(camPos, view, projection);

				// Swap to default batch.
				renderer.mesh->SwapBatch();
				renderer.mesh->FillBatch(&renderDense[i], 1);
				renderer.mesh->Draw();
			}
			
			postProcModule.RenderEnd();
		}

		postProcModule.PostRender();
	}
}
