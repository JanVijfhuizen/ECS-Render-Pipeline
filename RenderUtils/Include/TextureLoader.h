#pragma once
#include <glad/glad.h>
#include <memory>
#include <string>

namespace rut
{
	class Texture;

	// Used to load in textures from file.
	class TextureLoader final
	{
	public:
		// Get an object containing a texture handle.
		[[nodiscard]] static std::unique_ptr<Texture> Load(const std::string& path);
	};

	// Used to define the texturing of a mesh.
	class Texture final
	{
		friend TextureLoader;

	public:
		explicit Texture(GLuint handle);
		~Texture();

		// Get the texture handle.
		[[nodiscard]] GLuint GetHandle() const;

	private:
		const GLint _handle = -1;
	};
}
