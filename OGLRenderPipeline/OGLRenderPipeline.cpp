#include "Cecsar.h"
#include "RenderSystem.h"
#include "WindowModule.h"
#include "ExampleFactory.h"
#include "MapSet.h"
#include "Camera.h"
#include "InverseEffect.h"
#include "Transform.h"
#include "TransformSystem.h"

int main()
{
	jecs::Cecsar cecsar{50};

	auto& windowModule = rpi::WindowModule::Get();

	// Construct some entity quad.
	auto& factory = rpi::example::ExampleFactory::Get();
	auto& transforms = jecs::SparseSet<rpi::Transform>::Get();

	const int32_t rSpawn = 10;
	glm::mat4 batchArr[rSpawn];
	for (int32_t i = 0; i < rSpawn; ++i)
	{
		const auto quad = cecsar.Spawn();
		factory.Construct(quad.index);
		auto& transform = transforms[quad.index];
		transform.position = { rand() % 4 - 2, rand() % 2 - 1, rand() % 4 - 2 };
		transform.rotation = { rand() % 360, rand() % 360, rand() % 360 };

		batchArr[i] = rpi::TransformSystem::GetMatrix(transform);
	}

	factory.GetMesh().UpdateInstanceBuffer(batchArr, rSpawn);
	rpi::RenderSystem::Batch batch;
	batch.size = rSpawn;
	
	// Construct a entity camera.
	const auto camera = cecsar.Spawn();
	auto& cameras = jecs::MapSet<rpi::Camera>::Get();
	auto& camComponent = cameras.Insert(camera.index);
	auto& camTransform = transforms.Insert(camera.index);

	rpi::PostEffect* inverseEffect = new rpi::InverseEffect;
	camComponent.postProcStack.push_back(inverseEffect);
	camComponent.postProcStack.push_back(inverseEffect);
	camComponent.postProcStack.push_back(inverseEffect);

	// Construct a entity camera.
	const auto camera2 = cecsar.Spawn();
	cameras.Insert(camera2.index);
	auto& cam2Transform = transforms.Insert(camera2.index);
	cam2Transform.position = { 0, 4, -4 };

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
		dt += .001f;
		camTransform.position.x = sin(dt) * 4;
		camTransform.position.z = cos(dt) * 4;
		camTransform.position.y = sin(dt / 2) + 2;

		lightTrans.position.x = cos(-dt) * .5f;
		lightTrans.position.z = sin(-dt) * .5f;
		lightTrans.position.y = .2f;

		rpi::RenderSystem::Update(&batch, 1);

		windowModule.EndFrame();
	}

	return 0;
}
