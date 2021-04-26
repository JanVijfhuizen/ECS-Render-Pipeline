#include "BasicRenderSystem.h"
#include "Cecsar.h"
#include "Modules/WindowModule.h"

int main()
{
	jecs::Cecsar cecsar{50};
	const auto& windowModule = rpi::WindowModule::Get();

	while(true)
	{
		bool quit = false;
		windowModule.BeginFrame(&quit);
		if (quit)
			break;

		BasicRenderSystem::Update();
		
		windowModule.EndFrame();
	}

	return 0;
}
