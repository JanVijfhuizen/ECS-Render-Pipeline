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

		for (int32_t i = 0; i < 2; ++i)
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
		glDeleteTextures(2, _textureBuffers);
		glDeleteFramebuffers(1, &_fbo);

		glDeleteVertexArrays(1, &_vao);
		glDeleteProgram(_program);
	}

	void PostEffectModule::RenderBegin(PostEffect* effects, const int32_t count)
	{
		_effects = effects;
		_effectCount = count;

		glBindBuffer(GL_FRAMEBUFFER, _fbo);
		BindTextureBuffer(_bufferOdd);
	}

	void PostEffectModule::RenderEnd()
	{
		// Iterate over all the post effects.
		bool odd = _bufferOdd;
		for (int32_t i = 0; i < _effectCount; ++i)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _textureBuffers[odd]);
			odd = !odd;

			BindTextureBuffer(odd);
			
			_effects[i].Use();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		glBindBuffer(GL_FRAMEBUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textureBuffers[odd]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void PostEffectModule::PostRender()
	{
		//glBindBuffer(GL_FRAMEBUFFER, 0);
		//glUseProgram(_program);

		// Draw.		
	}

	GLuint PostEffectModule::GetFbo() const
	{
		return _fbo;
	}

	void PostEffectModule::BindTextureBuffer(const int32_t index)
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + index);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void PostEffectModule::SetupShader()
	{
		// Define shader.
		const GLuint vert = ShaderLoader::CreateShader(
			"Resources/passthrough.vert", GL_VERTEX_SHADER);
		const GLuint frag = ShaderLoader::CreateShader(
			"Resources/passthrough.frag", GL_FRAGMENT_SHADER);
		_program = ShaderLoader::LinkShaders(vert, frag);
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
