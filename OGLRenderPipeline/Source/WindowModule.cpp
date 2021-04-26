#include "Modules/WindowModule.h"

#include <glad/glad.h>
#include <cassert>

namespace rpi
{
	float WindowModule::Settings::GetAspectRatio() const
	{
		return static_cast<float>(width) / height;
	}

	WindowModule::Observer::Observer()
	{
		_module = &Get();
		_module->_observers.push_back(this);
	}

	WindowModule::Observer::~Observer()
	{
		if (!_module)
			return;

		auto& observers = _module->_observers;
		observers.erase(std::remove(observers.begin(), observers.end(), 
			this), observers.end());
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
		glfwSetFramebufferSizeCallback(_window, ResizeCallback);

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

	void WindowModule::ResizeCallback(GLFWwindow* window, 
		const int32_t width, const int32_t height)
	{
		auto& settings = Get()._settings;
		settings.width = width;
		settings.height = height;

		glViewport(0, 0, width, height);
	}
}
