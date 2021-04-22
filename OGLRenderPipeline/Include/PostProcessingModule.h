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

		void RenderNext();
		void PostRender();

	protected:
		void OnWindowResize(GLFWwindow* window, int32_t width, int32_t height) override;
	
	private:
		struct CameraBuffer final
		{
			GLuint fbo = 0;
			GLuint texture = 0;

			~CameraBuffer();
		};

		GLuint _program = 0;
		GLuint _vao = 0;

#define MAX_CAMERAS 16	
		GLuint _camCount = 0;
		CameraBuffer* _camBuffers = nullptr;
		int32_t _currentCamIndex = 0;

		void GenerateBuffers();
		void DeleteBuffers() const;

		void SetupShader();
		void SetupModel();
	};
}
