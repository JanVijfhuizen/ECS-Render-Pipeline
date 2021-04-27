#pragma once

// Just a basic example of how a render system that utilizes the modules and cecsar could look like.
class BasicRenderSystem final
{
public:
	// Render the models using multiple cameras and their respective post processing stacks.
	static void Update();
};