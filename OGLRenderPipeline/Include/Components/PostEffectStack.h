#pragma once
#include <vector>

namespace rpi
{
	class PostEffect;

	struct PostEffectStack final
	{
		std::vector<PostEffect> effects{};
	};
}
