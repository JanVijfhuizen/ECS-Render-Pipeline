#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include "Vertex.h"

bool ObjLoader::CreateModel(const std::string& filename, 
	std::vector<rut::Vertex>& vertices, 
	std::vector<int32_t>& indices)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		fprintf(stdin, "Couldn't load model: %s\n", filename.c_str());
		return false;
	}

	std::string line;

	// Forward to the actual content.
	while (std::getline(file, line))
		if (line[0] == 'o')
			break;

	std::vector<glm::vec3> positions{};
	std::vector<glm::vec2> texCoords{};
	std::vector<glm::vec3> normals{};

	while (std::getline(file, line))
	{
		// Used to check type of parsed data.
		char pre1 = line[0];
		char pre2 = line[1];

		// Used for parsing.
		std::istringstream iss(line);
		std::string prefix;
		glm::vec3 vec3;
		char c;
		int i;
		rut::Vertex vert;

		int32_t index = 0;

		switch (pre1)
		{
			// Vertex attribute.
		case 'v':
			switch (pre2)
			{
				// Position.
			case ' ':
				iss >> prefix >> vec3.x >> vec3.y >> vec3.z;
				positions.push_back(vec3);
				break;
				// Texture coordinates.
			case 't':
				iss >> prefix >> vec3.x >> vec3.y;
				texCoords.emplace_back(vec3);
				break;
				// Normal.
			case 'n':
				iss >> prefix >> vec3.x >> vec3.y >> vec3.z;
				texCoords.emplace_back(vec3);
				break;
			default:
				fprintf(stdin, "Obj file is corrupted: %s\n", filename.c_str());
				break;
			}
			break;
			// Indices.
		case 'f':
			while (iss >> c)
			{
				if (c == '/')
					continue;
				if(c == ' ')
				{
					index = 0;
					continue;
				}

				i = static_cast<int32_t>(c);
				switch (index)
				{
				case 0:
					vert.position = positions[i];
					break;
				case 1:
					vert.texCoords = texCoords[i];
					break;
				case 2:
					vert.normal = normals[i];
					vertices.push_back(vert);
					break;
				default:
					fprintf(stdin, "Obj file is corrupted: %s\n", filename.c_str());
					break;
				}
			}
			break;
			// Either comments or material stuff.
			// You can define your material implementation here.
		default:
			break;
		}
	}

	return true;
}
