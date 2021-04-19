#include "RenderSystem.h"
#include <iostream>
#include <cassert>
#include <glad/glad.h>

namespace rpi
{
	RenderSystem::RenderSystem(const Settings& settings) : _settings(settings)
	{	
		// Initialize GLFW.
		glfwInit();

		// Create window.
		_window = glfwCreateWindow(
			settings.width, settings.height, settings.title.c_str(),
			nullptr, nullptr);
		assert(_window);
		glfwWindowHint(GLFW_OPENGL_PROFILE, settings.profile);

		// Set window as current and setup GLAD.
		glfwMakeContextCurrent(_window);
		const bool gladLoaded = gladLoadGLLoader(
			reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		assert(gladLoaded);

		// Setup viewport and viewport related callbacks.
		glViewport(0, 0, settings.width, settings.height);
		glfwSetFramebufferSizeCallback(_window, ResizeCallback);
	}

	RenderSystem::~RenderSystem()
	{
		glfwTerminate();
	}

	void RenderSystem::BeginFrame(bool* quit) const
	{
		*quit = glfwWindowShouldClose(_window);
		glfwPollEvents();
	}

	void RenderSystem::EndFrame()
	{
	}

	RenderSystem::Settings RenderSystem::GetSettings() const
	{
		return _settings;
	}

	void RenderSystem::ResizeCallback(GLFWwindow* window,
		const int32_t width, const int32_t height)
	{
		auto& settings = Get()._settings;
		settings.width = width;
		settings.height = height;

		glViewport(0, 0, width, height);
	}
}
