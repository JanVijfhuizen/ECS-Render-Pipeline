#pragma once
#include "Module.h"
#include "WindowModule.h"

namespace rpi
{
	// Cecsar class that extends on the window module.
	class CeWindowModule final : public rut::WindowModule,
		public jecs::Module<CeWindowModule>
	{

	};
}
