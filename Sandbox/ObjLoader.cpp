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

	// Store all the vertex data.
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	// Use the set to check if it already exist, and use the vector to quickly get it by index.
	std::unordered_set<std::string> vertSet;
	std::vector<rut::Vertex> vertVec;

	// Store the vertex indices of the current face.
	std::vector<int32_t> faceInds;

	while (std::getline(file, line))
	{
		// Used to check type of parsed data.
		char pre1 = line[0];
		char pre2 = line[1];

		// Pretty annoying but I can't declare variables in the switch cases.

		// Used for parsing.
		std::istringstream iss(line);
		std::string prefix;

		// Used to parse in vertex attributes.
		glm::vec3 vec3;
		// s1 = p/t/n, s2 = p t n
		std::string s1, s2;
		// Used for parsing the faces, where / is used.
		std::istringstream iss2;

		// Used to iterate over the hashset.
		std::unordered_set<std::string>::iterator it;
		int32_t i;

		// Used to parse in indices information.
		int32_t posIndex, texIndex, norIndex;
		rut::Vertex vertex;

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
			// Faces.
		case 'f':
			// Get the faces as a string.
			iss >> prefix;
			faceInds.clear();

			// Collect all the indices.
			while(iss >> s1)
			{
				i = vertSet.size();
				it = vertSet.find(s1);

				// If vertex has been defined before.
				if (it != vertSet.end())
					i = std::distance(vertSet.begin(), it);
				// Otherwise add it to the set.
				else
				{
					// Deconstruct string into a vertex.
					s2 = s1;
					std::replace(s2.begin(), s2.end(), '/', ' ');
					iss2 = std::istringstream(s2);

					iss2 >> posIndex >> texIndex >> norIndex;
					vertex.position = positions[posIndex - 1];
					vertex.texCoords = texCoords[texIndex - 1];
					vertex.normal = normals[norIndex - 1];

					vertSet.insert(s1);
					vertVec.push_back(vertex);
				}

				// Collect the indices that make up the current face.
				faceInds.push_back(i);
			}

			// Now convert the face indices into triangles.
			i = vertVec.size();
			switch (i)
			{
				// Triangle.
			case 3:
				break;
				// Quad.
			case 4:
				break;
				// Ngon.
			default:
				break;
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
