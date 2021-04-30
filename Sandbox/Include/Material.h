#pragma once
#include "glm/ext.hpp"

struct Material
{
	glm::vec3 color{1};
	float specularity = 32;
	
	GLuint diffuseTex = 0;
	GLuint normalTex = 0;
	GLuint specularityTex = 0;
};
