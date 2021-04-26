#pragma once

namespace rpi
{
	class Mesh;
	class Shader;

	struct Model final
	{
		Shader* shader = nullptr;
		Mesh* mesh = nullptr;
		char layer = 0b000001;
	};	
}
