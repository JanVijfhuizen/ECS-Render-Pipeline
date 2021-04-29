#include "BasicRenderSystem.h"

#include "BakedTransform.h"
#include "MapSet.h"
#include "SparseSet.h"
#include "Components/Camera.h"
#include "Components/Model.h"
#include "Components/PostEffectStack.h"
#include "Components/Transform.h"
#include "Meshes/CeMesh.h"
#include "Shaders/CeShader.h"
#include "Systems/CameraSystem.h"
#include "Modules/CePostEffectModule.h"
#include "Systems/TransformSystem.h"

void BasicRenderSystem::Update()
{
	// Grab the required component sets.
	auto& cameras = jecs::MapSet<rpi::Camera>::Get();
	auto& models = jecs::SparseSet<rpi::Model>::Get();
	
	const auto& transforms = jecs::SparseSet<rpi::Transform>::Get();
	auto& bakedTransforms = jecs::SparseSet<BakedTransform>::Get();
	auto& postEffectStacks = jecs::MapSet<rpi::PostEffectStack>::Get();
	
	// Grab the required modules.
	auto& postEffectModule = rpi::CePostEffectModule::Get();

	// Forward all model matrices to the baked transforms.
	for (const auto [model, index] : models)
	{
		const auto& transform = transforms[index];
		// If the index is not empty, it just returns the value at that index.
		// So this is used more like a GetOrCreate type of method.
		auto& bakedTransform = bakedTransforms.Insert(index);
		// No need to update static geometry.
		if (bakedTransform.isStatic)
			continue;

		// Update the matrix for non-static transforms. This has a couple of advantages.
		// 1: We don't have to recalculate the matrix for every single camera, we just do it once.
		// 2: I can't overstress how expensive this method is. Making things static is THE way to gain performance,
		// aside from batch rendering techniques.
		bakedTransform.model = rpi::TransformSystem::GetMatrix(transform);
	}

	// Iterate over all the cameras.
	for (const auto [camera, camIndex] : cameras)
	{
		rut::PostEffect** postEffects = nullptr;
		int32_t postEffectsSize = 0;

		// Check for any post processing effects.
		if (postEffectStacks.Contains(camIndex))
		{
			auto& effects = postEffectStacks[camIndex].effects;
			postEffects = effects.data();
			postEffectsSize = effects.size();
		}

		// Write to the post effect buffers instead of the screen.
		postEffectModule.RenderBegin(postEffects, postEffectsSize);
		
		const auto eye = transforms[camIndex].position;
		const auto view = rpi::CameraSystem::GetView(camIndex);
		const auto projection = rpi::CameraSystem::GetProjection(camIndex);

		for (const auto [model, modIndex] : models)
		{
			if (rpi::CameraSystem::Ignore(camera, model.layer))
				continue;

			model.shader->Use(modIndex, eye, view, projection);
			model.mesh->Draw(modIndex);
		}

		// Use the post effects and draw the buffer on top of the other camera's output.
		postEffectModule.RenderEnd();
	}

	// Draw the post effect's final image to the screen.
	postEffectModule.PostRender();
}
