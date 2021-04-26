#pragma once
#include <vector>

namespace rut
{
	class PostEffect;
}

namespace rpi
{
	struct PostEffectStack final
	{
		std::vector<rut::PostEffect> effects{};
	};
}
