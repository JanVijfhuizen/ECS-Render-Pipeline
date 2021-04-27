#include "BasicRenderSystem.h"

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
#include "PostEffect.h"

void BasicRenderSystem::Update()
{
	auto& cameras = jecs::MapSet<rpi::Camera>::Get();
	auto& models = jecs::SparseSet<rpi::Model>::Get();
	
	auto& postEffectStacks = jecs::MapSet<rpi::PostEffectStack>::Get();
	const auto& transforms = jecs::SparseSet<rpi::Transform>::Get();

	auto& postEffectModule = rpi::CePostEffectModule::Get();

	for (const auto [camera, camIndex] : cameras)
	{
		rut::PostEffect** postEffects = nullptr;
		int32_t postEffectsSize = 0;
		
		if (postEffectStacks.Contains(camIndex))
		{
			auto& effects = postEffectStacks[camIndex].effects;
			postEffects = effects.data();
			postEffectsSize = effects.size();
		}

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

		postEffectModule.RenderEnd();
	}

	postEffectModule.PostRender();
}
