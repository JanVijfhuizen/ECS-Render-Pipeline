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
			
		};

		struct Ambient final
		{
			
		};

		using Type = std::variant<Point, Direction, Ambient>;

		glm::vec3 diffuse{ .5 };

		Type type = Point();
	};
}
