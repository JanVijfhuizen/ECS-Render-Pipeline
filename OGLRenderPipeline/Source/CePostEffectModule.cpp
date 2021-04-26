#include "Modules/CePostEffectModule.h"

#include "Modules/CeWindowModule.h"

namespace rpi
{
	CePostEffectModule::CePostEffectModule() :
		PostEffectModule(CeWindowModule::Get().GetSettings().GetResolution())
	{
		
	}
}
