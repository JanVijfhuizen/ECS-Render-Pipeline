#pragma once
#include <vector>

namespace rut
{
	class PostEffect;
}

namespace rpi
{
	// Component that extends the camera component with post processing effects.
	struct PostEffectStack final
	{
		// The effects used, and the order of which they're being applied.
		std::vector<rut::PostEffect*> effects{};
	};
}
