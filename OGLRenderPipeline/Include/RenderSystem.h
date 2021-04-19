#pragma once
#include "Module.h"
#include <glfw3.h>

namespace rpi
{
	struct Camera final
	{
		
	};

	struct Renderer final
	{

	};

	class RenderSystem final : public jecs::Module<RenderSystem>
	{
	public:
		struct Settings final
		{
			std::string title = "Render Pipeline";

			int32_t width = 640;
			int32_t height = 480;

			int32_t profile = GLFW_OPENGL_CORE_PROFILE;
		};

		explicit RenderSystem(const Settings& settings = {});
		~RenderSystem();

		void BeginFrame(bool* quit) const;
		void EndFrame();

		[[nodiscard]] Settings GetSettings() const;

	private:
		Settings _settings{};
		GLFWwindow* _window = nullptr;

		static void ResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
	};
}
