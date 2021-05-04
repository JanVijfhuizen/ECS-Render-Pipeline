#pragma once
#include "Module.h"

// Just a basic example of how a render system that utilizes the modules and cecsar could look like.
class BasicRenderSystem final : public jecs::Module<BasicRenderSystem>
{
public:
	// Anti Aliasing MSAA.
	int32_t aaSamples = 4;

	// Render the models using multiple cameras and their respective post processing stacks.
	void Update();
};