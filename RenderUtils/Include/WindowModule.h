#pragma once
#include <glfw3.h>
#include <string>
#include <glm/vec3.hpp>

namespace rut
{
	class WindowModule
	{
	public:
		struct Settings final
		{
			std::string title = "Render Pipeline";

			int32_t width = 640;
			int32_t height = 480;

			int32_t profile = GLFW_OPENGL_CORE_PROFILE;
			int32_t version = 4;

			[[nodiscard]] float GetAspectRatio() const;
		};

		glm::vec3 clearColor{.2f, .4f, .2f};

		explicit WindowModule(const Settings& settings = {});
		~WindowModule();

		void BeginFrame(bool* quit) const;
		void EndFrame() const;

		[[nodiscard]] Settings GetSettings() const;

	private:
		Settings _settings{};
		GLFWwindow* _window = nullptr;
	};
}
