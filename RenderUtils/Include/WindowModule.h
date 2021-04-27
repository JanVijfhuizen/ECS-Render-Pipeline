#pragma once
#include <glfw3.h>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace rut
{
	// Used to create and update a window.
	class WindowModule
	{
	public:
		// Used to define the type of window instantiated.
		struct Settings final
		{
			std::string title = "Render Pipeline";

			int32_t width = 640;
			int32_t height = 480;

			int32_t profile = GLFW_OPENGL_CORE_PROFILE;
			int32_t version = 4;

			bool vSync = false;

			[[nodiscard]] glm::vec2 GetResolution() const;
			[[nodiscard]] float GetAspectRatio() const;
		};

		// Color used to clear the screen after each update.
		glm::vec3 clearColor{.2f, .4f, .2f};

		explicit WindowModule(const Settings& settings = {});
		~WindowModule();

		// Call this at the start of a frame.
		void BeginFrame(bool* quit) const;
		// Call this at the end of a frame.
		void EndFrame() const;

		// Get settings used to define this window.
		[[nodiscard]] Settings GetSettings() const;

	private:
		Settings _settings{};
		GLFWwindow* _window = nullptr;
	};
}
