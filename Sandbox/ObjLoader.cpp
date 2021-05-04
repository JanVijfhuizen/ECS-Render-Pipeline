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

	std::vector<int32_t> faceIndices;
	std::unordered_set<VertInfo, Hasher> vertSet;

	while (std::getline(file, line))
	{
		// Used to check type of parsed data.
		char pre1 = line[0];

		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		// Vertex attribute.
		if(pre1 == 'v')
		{
			char pre2 = line[1];
			glm::vec3 vec3;

			switch (pre2)
			{
				// Position.
			case ' ':
				iss >> vec3.x >> vec3.y >> vec3.z;
				positions.push_back(vec3);
				break;
				// Texture coordinates.
			case 't':
				iss >> vec3.x >> vec3.y;
				texCoords.emplace_back(vec3);
				break;
				// Normal.
			case 'n':
				iss >> vec3.x >> vec3.y >> vec3.z;
				normals.push_back(vec3);
				break;
			default:
				fprintf(stdin, "Obj file is corrupted: %s\n", filename.c_str());
				break;
			}
		}
		// Faces.
		else if(pre1 == 'f')
		{
			faceIndices.clear();
			std::string face;

			// Collect all the indices.
			while (iss >> face)
			{
				int32_t index = vertices.size();
				auto it = vertSet.find({ face });

				// If vertex has been defined before.
				if (it != vertSet.end())
					index = it->index;
				// Otherwise add it to the set.
				else
				{
					vertSet.insert({face, index});

					// Deconstruct string into a vertex.		
					std::replace(face.begin(), face.end(), '/', ' ');

					std::istringstream iss2(face);
					int32_t posIndex, texIndex, norIndex;
					iss2 >> posIndex >> texIndex >> norIndex;

					rut::Vertex vert;

					vert.position = positions[posIndex - 1];
					vert.texCoords = texCoords[texIndex - 1];
					vert.normal = normals[norIndex - 1];

					vertices.push_back(vert);
				}

				faceIndices.push_back(index);
			}

			// Now convert the face indices into triangles.
			const int32_t index = faceIndices.size();
			switch (index)
			{
				// Triangle.			
			case 3:
				for (auto& ind : faceIndices)
					indices.push_back(ind);
				break;
				// Quad.
			case 4:
				// Triangle #1.
				for (int32_t i = 0; i < 2; ++i)
					indices.push_back(faceIndices[i]);
				indices.push_back(faceIndices[3]);

				// Triangle #2.
				for (int32_t i = 1; i < 4; ++i)
					indices.push_back(faceIndices[i]);
				break;
				// Ngon.
			default:
				assert(false);
				break;
			}
		}
	}

	return true;
}

bool ObjLoader::VertInfo::operator==(const VertInfo& other) const
{
	return comparer == other.comparer;
}

bool ObjLoader::VertInfo::operator!=(const VertInfo& other) const
{
	return comparer != other.comparer;
}

size_t ObjLoader::Hasher::operator()(const VertInfo& vert) const
{
	return _hasher(vert.comparer);
}
