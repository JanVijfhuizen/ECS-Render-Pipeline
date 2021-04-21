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
	auto& transforms = jecs::SparseSet<rpi::Transform>::Get();

	for (int32_t i = 0; i < 1; ++i)
	{
		const auto quad = cecsar.Spawn();
		factory.Construct(quad.index);
		auto& transform = transforms[quad.index];
		//transform.position = { rand() % 4 - 2, rand() % 2 - 1, rand() % 4 - 2 };
		//transform.rotation = { rand() % 360, rand() % 360, rand() % 360 };
		transform.scale = glm::vec3{ 8 };
	}

	// Construct a entity camera.
	const auto camera = cecsar.Spawn();
	auto& cameras = jecs::MapSet<rpi::Camera>::Get();
	cameras.Insert(camera.index);
	auto& camTransform = transforms.Insert(camera.index);

	// Construct a light.
	const auto light = cecsar.Spawn();
	auto& lights = jecs::MapSet<rpi::Light>::Get();
	auto& lightComponent = lights.Insert(light.index);
	lightComponent.type = rpi::Light::Point();
	auto& lightTrans = transforms.Insert(light.index);

	float dt = 0;

	while(true)
	{
		bool quit = false;
		windowModule.BeginFrame(&quit);
		if (quit)
			break;

		// Just some testing code.
		dt += .002f;
		camTransform.position.x = sin(1) * 4;
		camTransform.position.z = cos(1) * 4;
		camTransform.position.y = sin(1 / 2) + 2;

		lightTrans.position.x = sin(-dt) * 2;
		lightTrans.position.z = cos(-dt) * 2;
		lightTrans.position.y = .2f;

		rpi::RenderSystem::Update();

		windowModule.EndFrame();
	}

	return 0;
}
