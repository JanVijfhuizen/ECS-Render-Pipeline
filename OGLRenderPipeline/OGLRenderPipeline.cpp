#include "Cecsar.h"
#include <glfw3.h>
#include "RenderSystem.h"
#include "SparseSet.h"

int main()
{
	jecs::Cecsar cecsar;

	glfwInit();

	auto& renderSystem = rpi::RenderSystem::Get();
	auto& renderers = jecs::SparseSet<rpi::Renderer>::Get();

	while(true)
	{
		bool quit = false;
		renderSystem.BeginFrame(&quit);
		if (quit)
			break;

		renderSystem.EndFrame();
	}

	return 0;
}
