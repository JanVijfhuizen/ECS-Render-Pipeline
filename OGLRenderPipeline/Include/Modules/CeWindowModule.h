#pragma once
#include "Module.h"
#include "WindowModule.h"

namespace rpi
{
	class CeWindowModule final : public rut::WindowModule,
		public jecs::Module<CeWindowModule>
	{
	public:

	};
}
