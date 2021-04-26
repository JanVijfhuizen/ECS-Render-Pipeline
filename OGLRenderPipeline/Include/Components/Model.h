#pragma once

namespace rut
{
	class Mesh;
	class Shader;
}

namespace rpi
{
	struct Model final
	{
		rut::Shader* shader = nullptr;
		rut::Mesh* mesh = nullptr;
		char layer = 0b000001;
	};	
}
