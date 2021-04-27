#pragma once
#include <glad/glad.h>
#include "PostEffect.h"

namespace rpi
{
	// Post effect that inverses the colors of an image. Note that it does not inverse the alpha values.
	class InverseEffect final : public rut::PostEffect
	{
	public:
		InverseEffect();
		~InverseEffect();

		// Once activated, the following meshes will be drawn using this shader.
		void Use() override;

	private:
		GLuint _program = 0;
	};
}
