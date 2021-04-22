﻿#pragma once
#include "Module.h"
#include <glfw3.h>
#include <glm/vec3.hpp>

namespace rpi
{
	class WindowModule final : public jecs::Module<WindowModule>
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

		class Observer
		{
		public:
			Observer();
			virtual ~Observer();

		protected:
			virtual void OnWindowResize(GLFWwindow* window, int32_t width, int32_t height) = 0;

		private:
			WindowModule* _module = nullptr;
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

		std::vector<Observer*> _observers{};

		static void ResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
	};
}
