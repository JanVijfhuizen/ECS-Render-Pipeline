#pragma once
#include "Module.h"
#include "PostEffectModule.h"

namespace rpi
{
	class CePostEffectModule : public rut::PostEffectModule,
		public jecs::Module<CePostEffectModule>
	{
	public:
		CePostEffectModule();
	};
}
