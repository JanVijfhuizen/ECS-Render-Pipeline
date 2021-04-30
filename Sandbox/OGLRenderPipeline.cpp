#include <iostream>

#include "BasicRenderSystem.h"
#include "Cecsar.h"
#include "InverseEffect.h"
#include "Light.h"
#include "MapSet.h"
#include "SoASet.h"
#include "SparseSet.h"
#include "TestFactory.h"
#include "Components/Camera.h"
#include "Components/PostEffectStack.h"
#include "Components/Transform.h"
#include "Modules/CeWindowModule.h"

int main()
{
	// This is just some testing code, but it does show how you can use the render utils/pipeline to render your own stuff.
	jecs::Cecsar cecsar{ 50 };

	const auto& windowModule = rpi::CeWindowModule::Get();
	auto& cameras = jecs::MapSet<rpi::Camera>::Get();
	auto& transforms = jecs::SparseSet<rpi::Transform>::Get();

	// Add a test object.
	for (int32_t i = 0; i < 20; ++i)
	{
		const auto testObj = cecsar.Spawn();
		TestFactory::Get().Construct(testObj);

		auto& trans = transforms[testObj.index];
		trans.position = { .1f * (rand() % 100) - 5, .1f * (rand() % 100) - 5 , .1f * (rand() % 100) - 5 };
		trans.rotation = { rand() % 360, rand() % 360, rand() % 360 };
	}

	// Add some cameras.
	const auto cam1 = cecsar.Spawn();
	const auto cam2 = cecsar.Spawn();

	cameras.Insert(cam1.index);
	cameras.Insert(cam2.index);

	auto& postEffectStacks = jecs::MapSet<rpi::PostEffectStack>::Get();
	auto& stack = postEffectStacks.Insert(cam2.index);

	// Add some post effects.
	rpi::InverseEffect effect;
	stack.effects.push_back(&effect);
	stack.effects.push_back(&effect);
	stack.effects.push_back(&effect);

	auto& cam1Trans = transforms.Insert(cam1.index);
	auto& cam2Trans = transforms.Insert(cam2.index);

	// Now add some lighting.
	auto& lights = jecs::MapSet<Light>::Get();
	auto& light = lights.Insert(cam2.index);
	light.diffuse *= 5;

	// Change the position of the first camera.
	cam1Trans.position.y = 1;
	cam1Trans.position.z = -8;

	// It's okay to be frame dependent. This is for testing purposes only.
	float time = 0;
	const float dt = 1.f / 1000;

	while (true)
	{
		bool quit = false;
		windowModule.BeginFrame(&quit);
		if (quit)
			break;

		// Move the second camera around in circles, and a bit up and down.
		time += dt;
		cam2Trans.position.x = sin(time) * 16;
		cam2Trans.position.y = sin(time * 2) * 2;
		cam2Trans.position.z = cos(time) * 16;

		BasicRenderSystem::Update();

		windowModule.EndFrame();
	}

	return 0;
}
