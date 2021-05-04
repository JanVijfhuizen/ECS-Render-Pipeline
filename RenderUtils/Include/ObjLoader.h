#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace rut
{
	struct Vertex;

	class ObjLoader final
	{
	public:
		static bool CreateModel(const std::string& filename,
			std::vector<Vertex>& vertices, std::vector<int32_t>& indices);

	private:
		struct VertInfo final
		{
			std::string comparer{};
			int32_t index = 0;

			bool operator ==(const VertInfo& other) const;
			bool operator !=(const VertInfo& other) const;
		};

		class Hasher final
		{
		public:
			size_t operator()(const VertInfo& vert) const;

		private:
			std::hash<std::string> _hasher;
		};
	};
}