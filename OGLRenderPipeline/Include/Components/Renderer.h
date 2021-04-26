#pragma once
#include "Meshes/Mesh.h"
#include "Shaders/Shader.h"

namespace rpi
{
	struct Renderer final
	{
		Mesh* mesh = nullptr;
		Shader* shader = nullptr;
		
		char layer = 0b000001;
		bool isCulled = false;
		bool isStatic = false;
	};
}
