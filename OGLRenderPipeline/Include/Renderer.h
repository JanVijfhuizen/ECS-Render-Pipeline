#pragma once
#include "Mesh.h"
#include "Shader.h"

namespace rpi
{
	struct Renderer final
	{
		Mesh* mesh = nullptr;
		Shader* shader = nullptr;
	};
}
