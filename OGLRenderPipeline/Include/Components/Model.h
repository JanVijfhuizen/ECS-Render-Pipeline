#pragma once

namespace rpi
{
	class CeShader;
	class CeMesh;
	
	struct Model final
	{
		CeShader* shader = nullptr;
		CeMesh* mesh = nullptr;
		char layer = 0b000001;
	};	
}
