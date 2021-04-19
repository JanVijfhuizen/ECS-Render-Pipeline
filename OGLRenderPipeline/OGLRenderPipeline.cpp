#include "Cecsar.h"
#include "RenderSystem.h"
#include "SparseSet.h"
#include "Renderer.h"
#include "WindowModule.h"

int main()
{
	jecs::Cecsar cecsar;

	auto& windowModule = rpi::WindowModule::Get();
	auto& renderSystem = rpi::RenderSystem::Get();
	auto& renderers = jecs::SparseSet<rpi::Renderer>::Get();

	while(true)
	{
		bool quit = false;
		windowModule.BeginFrame(&quit);
		if (quit)
			break;

		windowModule.EndFrame();
	}

	return 0;
}
