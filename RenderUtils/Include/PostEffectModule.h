#pragma once
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "glad/glad.h"

namespace rut
{
	class PostEffect;

	// Used to process and modify the final image of a camera with post processing effects.
	class PostEffectModule
	{
	public:
		// The final image will be send to this buffer. If none, it will be send to the screen.
		GLuint outputBuffer = 0;
		glm::vec4 clearColor{.1f, .1f, .1f, 0};
		
		explicit PostEffectModule(glm::vec2 resolution);
		virtual ~PostEffectModule();

		// Call this once a new camera has been assigned.
		virtual void RenderBegin(PostEffect** effects, int32_t count);
		// Call this after a camera has finished rendering.
		virtual void RenderEnd();

		// Call this at the end of a frame. This writes the buffer's data to the screen.
		virtual void PostRender();

		// Get the Frame Buffer Object which this model uses.
		[[nodiscard]] GLuint GetFbo() const;
	
	private:
		GLuint _fbo = 0;
		GLuint _textureBuffers[4]{};
		GLuint _depthBuffer = 0;
		
		GLuint _vao = 0;
		GLuint _programPass = 0;
		GLuint _programCom = 0;
		
		PostEffect** _effects = nullptr;
		int32_t _effectCount = 0;
		bool _odd = false;

		void BindTextureBuffer(int32_t index) const;
		
		void SetupShader();
		void SetupModel();
	};
}
