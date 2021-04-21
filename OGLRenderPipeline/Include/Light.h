#pragma once
#include <variant>
#include <glm/vec3.hpp>

namespace rpi
{
	struct Light final
	{
		struct Point final
		{
			
		};

		struct Direction final
		{
			glm::vec3 forward{0, 0, 1};
		};

		struct Ambient final
		{
			
		};

		using Type = std::variant<Point, Direction, Ambient>;

		glm::vec3 diffuse{ 1 };

		Type type = Point();
	};
}
