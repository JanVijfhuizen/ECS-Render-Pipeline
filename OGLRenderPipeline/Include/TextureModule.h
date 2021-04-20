#pragma once
#include "Module.h"
#include <glad/glad.h>

namespace rpi
{
	class TextureModule final : public jecs::Module<TextureModule>
	{
	public:
		static GLuint Load(const std::string& path);
	};
}
