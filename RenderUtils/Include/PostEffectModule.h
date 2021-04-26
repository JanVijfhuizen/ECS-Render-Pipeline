#pragma once
#include <cstdint>
#include <glm/vec2.hpp>
#include "glad/glad.h"

namespace rut
{
	class PostEffect;

	class PostEffectModule
	{
	public:
		explicit PostEffectModule(glm::vec2 resolution);
		~PostEffectModule();
		
		void RenderBegin(PostEffect** effects, int32_t count);
		void RenderEnd();

		void PostRender();

		[[nodiscard]] GLuint GetFbo() const;
	
	private:
		GLuint _fbo = 0;
		GLuint _textureBuffers[2]{};
		
		GLuint _vao = 0;
		GLuint _program = 0;

		PostEffect** _effects = nullptr;
		int32_t _effectCount = 0;

		static void BindTextureBuffer(int32_t index);
		
		void SetupShader();
		void SetupModel();
	};
}
