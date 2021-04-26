#include "BasicRenderSystem.h"
#include "Cecsar.h"
#include "MapSet.h"
#include "SparseSet.h"
#include "TestFactory.h"
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Modules/WindowModule.h"

int main()
{
	jecs::Cecsar cecsar{50};
	
	const auto& windowModule = rpi::WindowModule::Get();
	auto& cameras = jecs::MapSet<rpi::Camera>::Get();
	auto& transforms = jecs::SparseSet<rpi::Transform>::Get();

	// Add a test object.
	for (int32_t i = 0; i < 20; ++i)
	{
		const auto testObj = cecsar.Spawn();
		TestFactory::Get().Construct(testObj);

		auto& trans = transforms[testObj.index];
		trans.position = { .1f * (rand() % 20), .1f * (rand() % 20) , .1f * (rand() % 20) };
		trans.rotation = { rand() % 360, rand() % 360, rand() % 360 };
	}

	// Add some cameras.
	const auto cam1 = cecsar.Spawn();
	const auto cam2 = cecsar.Spawn();

	cameras.Insert(cam1.index);
	cameras.Insert(cam2.index);

	auto& cam1Trans = transforms.Insert(cam1.index);
	auto& cam2Trans = transforms.Insert(cam2.index);

	cam1Trans.position.z = -4;

	// It's okay to be frame dependent. This is for testing purposes only.
	float time = 0;
	const float dt = 1.f / 1000;
	
	while(true)
	{
		bool quit = false;
		windowModule.BeginFrame(&quit);
		if (quit)
			break;

		time += dt;
		cam2Trans.position.x = sin(time) * 4;
		cam2Trans.position.y = sin(time * 2) * 2;
		cam2Trans.position.z = cos(time) * 4;

		BasicRenderSystem::Update();
		
		windowModule.EndFrame();
	}

	return 0;
}
