#pragma once
#include <glad/glad.h>
#include <memory>
#include <string>

namespace rpi
{
	class Texture;

	class TextureLoader final
	{
	public:
		[[nodiscard]] static std::unique_ptr<Texture> Load(const std::string& path);
	};

	class Texture final
	{
		friend TextureLoader;

	public:
		explicit Texture(GLuint handle);
		~Texture();

		[[nodiscard]] GLuint GetHandle() const;

	private:
		const GLint _handle = -1;
	};
}
