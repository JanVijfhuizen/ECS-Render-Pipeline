#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace rut 
{
	struct Vertex;
}

class ObjLoader final 
{
public:
	static bool CreateModel(const std::string& filename, 
		std::vector<rut::Vertex>& vertices, std::vector<int32_t>& indices);
};
