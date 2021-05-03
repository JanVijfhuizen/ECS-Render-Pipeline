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
	jecs::Cecsar cecsar{ 1000 };

	const auto& windowModule = rpi::CeWindowModule::Get();
	auto& renderSystem = BasicRenderSystem::Get();
	auto& factory = TestFactory::Get();

	auto& cameras = jecs::MapSet<rpi::Camera>::Get();
	auto& transforms = jecs::SparseSet<rpi::Transform>::Get();

	// Add a test object.
	for (int i = 0; i < 20; ++i)
		for (int32_t j = 0; j < 20; ++j)
		{
			const auto testObj = cecsar.Spawn();
			factory.Construct(testObj);

			auto& trans = transforms[testObj.index];
			trans.position = glm::vec3{ i, j, 0 } - glm::vec3(10, 10, 0);
		}

	// Add some cameras.
	const auto cam1 = cecsar.Spawn();
	const auto cam2 = cecsar.Spawn();

	auto& camera1 = cameras.Insert(cam1.index);
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
	auto& light = lights.Insert(cam1.index);
	light.diffuse *= 10;

	// Change the position of the first camera.
	cam1Trans.position.y = 6;
	cam1Trans.position.z = -8;
	camera1.target = { 0, 6, 0 };

	// Spawn one big object.
	const auto testObj = cecsar.Spawn();
	factory.Construct(testObj);
	auto& trans = transforms[testObj.index];
	trans.position = glm::vec3(0, 14, -2);
	trans.scale = glm::vec3{ 3, 3, 3 };

	// It's okay to be frame dependent. This is for testing purposes only.
	float time = 0;
	const float dt = 1.f / 500;

	while (true)
	{
		bool quit = false;
		windowModule.BeginFrame(&quit);
		if (quit)
			break;

		// Move the second camera around in circles, and a bit up and down.
		time += dt;
		cam2Trans.position.x = sin(time) * 64;
		cam2Trans.position.y = sin(time * 2) * 2;
		cam2Trans.position.z = cos(time) * 64;

		renderSystem.Update();

		windowModule.EndFrame();
	}

	return 0;
}
