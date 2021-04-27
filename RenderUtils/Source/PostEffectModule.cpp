#include "PostEffectModule.h"

#include <glm/vec3.hpp>
#include "PostEffect.h"
#include "ShaderLoader.h"

namespace rut
{
	PostEffectModule::PostEffectModule(const glm::vec2 resolution)
	{
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		for (int32_t i = 0; i < 4; ++i)
		{
			auto& buffer = _textureBuffers[i];

			glGenTextures(1, &buffer);
			glBindTexture(GL_TEXTURE_2D, buffer);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y,
				0, GL_RGBA, GL_FLOAT, nullptr);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
				GL_TEXTURE_2D, buffer, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		SetupShader();
		SetupModel();
	}

	PostEffectModule::~PostEffectModule()
	{
		glDeleteTextures(4, _textureBuffers);
		glDeleteFramebuffers(1, &_fbo);

		glDeleteVertexArrays(1, &_vao);
		glDeleteProgram(_programPass);
		glDeleteProgram(_programCom);
	}

	void PostEffectModule::RenderBegin(PostEffect** effects, const int32_t count)
	{
		_effects = effects;
		_effectCount = count;
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		
		BindTextureBuffer(0);
	}

	void PostEffectModule::RenderEnd()
	{
		glBindVertexArray(_vao);
		glActiveTexture(GL_TEXTURE0);

		// Iterate over all the post effects.
		bool odd = false;
		for (int32_t i = 0; i < _effectCount; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, _textureBuffers[odd]);
			odd = !odd;

			BindTextureBuffer(odd);
			
			_effects[i]->Use();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		// Draw on top of final image.
		glUseProgram(_programCom);
		
		glBindTexture(GL_TEXTURE_2D, _textureBuffers[odd]);	
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _textureBuffers[2 + _odd]);
		
		BindTextureBuffer(3 - _odd);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		
		_odd = !_odd;
	}

	void PostEffectModule::PostRender()
	{
		glBindVertexArray(_vao);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glUseProgram(_programPass);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textureBuffers[2 + _odd]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		BindTextureBuffer(2 + _odd);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint PostEffectModule::GetFbo() const
	{
		return _fbo;
	}

	void PostEffectModule::BindTextureBuffer(const int32_t index)
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + index);
		glClearColor(0, 1, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void PostEffectModule::SetupShader()
	{
		// Define passthrough shader.
		const GLuint vertPass = ShaderLoader::CreateShader(
			"Resources/passthrough.vert", GL_VERTEX_SHADER);
		const GLuint fragPass = ShaderLoader::CreateShader(
			"Resources/passthrough.frag", GL_FRAGMENT_SHADER);
		_programPass = ShaderLoader::LinkShaders(vertPass, fragPass);

		// Define combine shader.
		const GLuint vertCom = ShaderLoader::CreateShader(
			"Resources/passthrough.vert", GL_VERTEX_SHADER);
		const GLuint fragCom = ShaderLoader::CreateShader(
			"Resources/combine.frag", GL_FRAGMENT_SHADER);
		_programCom = ShaderLoader::LinkShaders(vertCom, fragCom);

		glUseProgram(_programCom);
		
		const auto a = glGetUniformLocation(_programCom, "a");
		const auto b = glGetUniformLocation(_programCom, "b");
		glUniform1i(a, 0);
		glUniform1i(b, 1);
	}

	void PostEffectModule::SetupModel()
	{
		// Define mesh.
		struct QuadVertex final
		{
			glm::vec3 pos;
			glm::vec2 texCoords;
		};

		QuadVertex vertices[] =
		{
			QuadVertex{{-1, -1, 0}, {0, 0}},
			QuadVertex{{1, -1, 0}, {1, 0}},
			QuadVertex{{1, 1, 0}, {1, 1}},
			QuadVertex{{-1, 1, 0}, {0, 1}},
		};

		GLuint indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		// Create model.
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Generate VBO for this specific model.
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(QuadVertex), 
			vertices, GL_STATIC_DRAW);

		// Generate EBO for this specific model.
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), 
			&indices[0], GL_STATIC_DRAW);

		// Position attribute.
		glVertexAttribPointer(0, 3, GL_FLOAT, 
			GL_FALSE, sizeof(QuadVertex), nullptr);
		glEnableVertexAttribArray(0);

		// Texture attribute.
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
			(void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);
	}
}
