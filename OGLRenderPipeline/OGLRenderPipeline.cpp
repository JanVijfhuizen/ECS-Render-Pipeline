#include "Cecsar.h"
#include "RenderSystem.h"
#include "WindowModule.h"
#include "ExampleFactory.h"
#include "MapSet.h"
#include "Camera.h"
#include "Transform.h"

int main()
{
	jecs::Cecsar cecsar{10};

	auto& windowModule = rpi::WindowModule::Get();

	// Construct some entity quad.
	auto& factory = rpi::example::ExampleFactory::Get();
	const auto quad = cecsar.Spawn();
	factory.Construct(quad.index);

	// Construct a entity camera.
	const auto camera = cecsar.Spawn();
	auto& cameras = jecs::MapSet<rpi::Camera>::Get();
	auto& transforms = jecs::SparseSet<rpi::Transform>::Get();
	cameras.Insert(camera.index);
	auto& camTransform = transforms.Insert(camera.index);

	float dt = 0;

	while(true)
	{
		bool quit = false;
		windowModule.BeginFrame(&quit);
		if (quit)
			break;

		dt += .002f;
		camTransform.position.x = sin(dt) * 4;
		camTransform.position.z = cos(dt) * 4;
		camTransform.position.y = sin(dt / 2) + 2;

		rpi::RenderSystem::Update();

		windowModule.EndFrame();
	}

	return 0;
}
