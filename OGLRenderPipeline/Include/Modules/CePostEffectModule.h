#pragma once
#include "Module.h"
#include "PostEffectModule.h"

namespace rpi
{
	// Cecsar class that extends on the post effect module.
	class CePostEffectModule : public rut::PostEffectModule,
		public jecs::Module<CePostEffectModule>
	{
	public:
		CePostEffectModule();
	};
}
