#pragma once
#include <variant>
#include <glm/vec3.hpp>

struct Light final
{
	struct Fallof final
	{
		float constant = 1;
		float linear = 0.09f;
		float quadratic = 0.032f;
	};

	struct Point final
	{
		Fallof fallof;
	};

	struct Directional final
	{

	};

	struct Spot final
	{
		Fallof fallof;
		float cutOff = 45;
	};

	struct Ambient final
	{

	};

	using Type = std::variant<Point, Directional, Spot, Ambient>;

	glm::vec3 diffuse{ 1 };
	Type type = Point();
};
