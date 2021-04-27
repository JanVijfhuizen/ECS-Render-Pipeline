#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace rut
{
	// Used to define a vertex it's attributes, used in meshes and shaders.
	struct Vertex final
	{
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec2 texCoords{};
	};
}