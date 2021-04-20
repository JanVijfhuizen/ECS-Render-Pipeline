#include "Cecsar.h"
#include "RenderSystem.h"
#include "WindowModule.h"
#include "ExampleFactory.h"

int main()
{
	jecs::Cecsar cecsar{10};

	auto& windowModule = rpi::WindowModule::Get();
	auto& renderSystem = rpi::RenderSystem::Get();

	auto& factory = rpi::example::ExampleFactory::Get();
	factory.Load();

	while(true)
	{
		bool quit = false;
		windowModule.BeginFrame(&quit);
		if (quit)
			break;

		renderSystem.Update();

		windowModule.EndFrame();
	}

	return 0;
}
