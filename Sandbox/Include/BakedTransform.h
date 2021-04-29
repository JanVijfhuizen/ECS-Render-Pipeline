#pragma once
#include <glm/ext.hpp>

struct BakedTransform final
{
	bool isStatic = false;
	glm::mat4 model{ 1 };
};
