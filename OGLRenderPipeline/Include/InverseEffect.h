﻿#pragma once
#include <glad/glad.h>
#include "PostEffect.h"

namespace rpi
{
	class InverseEffect final : public PostEffect
	{
	public:
		InverseEffect();
		~InverseEffect();
		
		void Use() override;

	private:
		GLuint _program = 0;
	};
}