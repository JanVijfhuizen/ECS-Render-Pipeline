#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include "Vertex.h"
#include <algorithm>
#include <unordered_set>

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

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	std::unordered_set<std::string> set;

	while (std::getline(file, line))
	{
		// Used to check type of parsed data.
		char pre1 = line[0];
		char pre2 = line[1];

		// Used for parsing.
		std::istringstream iss(line);
		std::string prefix;

		glm::vec3 vec3;
		std::string s;
		std::istringstream iss2;

		std::unordered_set<std::string>::iterator it;
		int32_t x, y, z;
		rut::Vertex vert;

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
				normals.push_back(vec3);
				break;
			default:
				fprintf(stdin, "Obj file is corrupted: %s\n", filename.c_str());
				break;
			}
			break;
			// Indices.
		case 'f':
			// Get the indices as a string.
			iss >> prefix;
			while(iss >> s)
			{
				// If vertex has been defined before.
				it = set.find(s);
				if (it != set.end())
				{
					indices.push_back(std::distance(set.begin(), it));
					continue;
				}

				// Now get the indices as integer values.
				std::replace(s.begin(), s.end(), '/', ' ');
				iss2 = std::istringstream(s);
				iss2 >> x >> y >> z;

				vert.position = positions[x - 1];
				vert.texCoords = texCoords[y - 1];
				vert.normal = normals[z - 1];

				set.insert(s);
				vertices.push_back(vert);
				indices.push_back(set.size() - 1);
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
