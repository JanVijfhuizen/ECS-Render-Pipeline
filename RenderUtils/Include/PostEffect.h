#pragma once
#include <cstdint>

namespace rut
{
	class PostEffect
	{
	public:
		virtual void Use(int32_t camIndex) = 0;
	};
}
