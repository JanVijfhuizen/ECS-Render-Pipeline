#include "WindowModule.h"

#include <glad/glad.h>
#include <cassert>

namespace rut
{
	glm::vec2 WindowModule::Settings::GetResolution() const
	{
		return { width, height };
	}

	float WindowModule::Settings::GetAspectRatio() const
	{
		return static_cast<float>(width) / height;
	}
	
	WindowModule::WindowModule(const Settings& settings)
	{
		// Initialize GLFW.
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.version);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.version);
		glfwWindowHint(GLFW_OPENGL_PROFILE, settings.profile);

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

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glfwSwapInterval(0);
	}

	WindowModule::~WindowModule()
	{
		glfwTerminate();
	}

	void WindowModule::BeginFrame(bool* quit) const
	{
		*quit = glfwWindowShouldClose(_window);
		if (*quit)
			return;

		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
	}

	void WindowModule::EndFrame() const
	{
		glFlush();
		glfwSwapBuffers(_window);
	}

	WindowModule::Settings WindowModule::GetSettings() const
	{
		return _settings;
	}
}
