#pragma once

namespace rpi
{
	class CeShader;
	class CeMesh;

	// Component used to render meshes.
	struct Model final
	{
		// Define shading.
		CeShader* shader = nullptr;
		// Define mesh.
		CeMesh* mesh = nullptr;
		// Cameras can be set up to ignore some layers.
		char layer = 0b000001;
	};	
}
