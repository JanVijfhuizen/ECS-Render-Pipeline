#pragma once
#include "Module.h"
#include "WindowModule.h"
#include <glad/glad.h>

namespace rpi
{
	class PostEffect;

	class PostProcessingModule final : public jecs::Module<PostProcessingModule>,
		public WindowModule::Observer
	{
	public:
		PostProcessingModule();
		~PostProcessingModule();

		void RenderBegin(std::vector<PostEffect*>& effects);
		void RenderEnd();
		
		void PostRender();

	protected:
		void OnWindowResize(GLFWwindow* window, int32_t width, int32_t height) override;
	
	private:
#define MAX_CAMERAS 16
		
		struct CameraBuffer final
		{
			GLuint fbo = 0;
			GLuint texture = 0;

			~CameraBuffer();
		};

		struct BufferQueue final
		{
			CameraBuffer* active = nullptr;
			CameraBuffer* inactive = nullptr;
			
			void Activate() const;
			void Swap();
		};

		GLuint _program = 0;
		GLuint _vao = 0;
		
		GLuint _camCount = 0;
		int32_t _currentCamIndex = 0;

		CameraBuffer* _camBuffers = nullptr;
		CameraBuffer _swappableBuffer;
		
		std::vector<PostEffect*>* _effects = nullptr;
		BufferQueue _bufferQueue{};

		void GenerateBuffers();
		void DeleteBuffers();

		static void GenerateBuffer(CameraBuffer& buffer);

		void SetupShader();
		void SetupModel();
	};
}
