#pragma once
#include "Module.h"
#include "WindowModule.h"
#include <glad/glad.h>

namespace rpi
{
	class PostProcessingModule final : public jecs::Module<PostProcessingModule>,
		public WindowModule::Observer
	{
	public:
		PostProcessingModule();
		~PostProcessingModule();

		void PreRender() const;
		void PostRender() const;

	protected:
		void OnWindowResize(GLFWwindow* window, int32_t width, int32_t height) override;
	
	private:
		GLuint _texture = 0;
		GLuint _program = 0;

		GLuint _vao = 0;
		GLuint _fbo = 0;

		void GenerateBuffers();
		void DeleteBuffers() const;
	};
}
