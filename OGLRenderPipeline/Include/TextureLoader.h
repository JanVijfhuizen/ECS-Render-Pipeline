#pragma once
#include "Module.h"
#include <glad/glad.h>

namespace rpi
{
	class TextureLoader final : public jecs::Module<TextureLoader>
	{
	public:
		static GLuint Load(const std::string& path);
	};
}
