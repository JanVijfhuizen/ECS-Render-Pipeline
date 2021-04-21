#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"

namespace rpi
{
	std::unique_ptr<Texture> TextureLoader::Load(const std::string& path)
	{
		GLuint handle;
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);

		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height,
			&nrChannels, 0);

		if (data)
		{
			const int32_t channels = nrChannels == 3 ? GL_RGB : GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, channels,
				width, height, 0, channels,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			printf("Failed to load texture.\n");
		stbi_image_free(data);

		return std::make_unique<Texture>(handle);
	}

	Texture::Texture(const GLuint handle) : _handle(handle)
	{

	}

	Texture::~Texture()
	{
		if (_handle == -1)
			return;
		const GLuint glHandle = _handle;
		glDeleteTextures(1, &glHandle);
	}

	GLuint Texture::GetHandle() const
	{
		return _handle;
	}
}
