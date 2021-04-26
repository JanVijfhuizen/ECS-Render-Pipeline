#pragma once
#include <cstdint>

namespace rut
{
	class PostEffect;

	class PostEffectModule
	{
	public:
		PostEffectModule();
		~PostEffectModule();
		
		void RenderBegin(PostEffect* effects, int32_t count);
		void RenderEnd();

		void PostRender();
	};
}
