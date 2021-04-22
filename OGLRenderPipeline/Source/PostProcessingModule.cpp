#include "PostProcessingModule.h"
#include <glad/glad.h>
#include "ShaderLoader.h"
#include <glm/vec2.hpp>

#include "PostEffect.h"

namespace rpi
{
	PostProcessingModule::PostProcessingModule()
	{
		GenerateBuffers();
		SetupShader();
		SetupModel();
	}

	PostProcessingModule::~PostProcessingModule()
	{
		DeleteBuffers();
		glDeleteShader(_program);
		glDeleteVertexArrays(1, &_vao);
	}

	void PostProcessingModule::RenderNext()
	{
		assert(_currentCamIndex < MAX_CAMERAS);
		const auto& buffer = _camBuffers[_currentCamIndex++];

		glBindFramebuffer(GL_FRAMEBUFFER, buffer.fbo);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void PostProcessingModule::PostRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(_program);

		for (int32_t i = 0; i < _currentCamIndex; ++i)
		{
			const auto& buffer = _camBuffers[i];

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, buffer.texture);
		}

		glUniform1i(_camCount, _currentCamIndex);
		_currentCamIndex = 0;
		
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void PostProcessingModule::RenderPostEffects(std::vector<PostEffect*>& effects)
	{
		glBindVertexArray(_vao);

		auto& buffer = _camBuffers[_currentCamIndex - 1];
		glActiveTexture(GL_TEXTURE0 );
		glBindTexture(GL_TEXTURE_2D, buffer.texture);

		for (auto& effect : effects)
		{
			effect->Use();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	void PostProcessingModule::OnWindowResize(GLFWwindow* window, 
		const int32_t width, const int32_t height)
	{
		DeleteBuffers();
		GenerateBuffers();
	}

	PostProcessingModule::CameraBuffer::~CameraBuffer()
	{
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &texture);
	}

	void PostProcessingModule::GenerateBuffers()
	{
		const auto& windowSettings = WindowModule::Get().GetSettings();
		_camBuffers = new CameraBuffer[MAX_CAMERAS];

		for (int32_t i = 0; i < MAX_CAMERAS; ++i)
		{
			auto& buffer = _camBuffers[i];

			glGenFramebuffers(1, &buffer.fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, buffer.fbo);

			glGenTextures(1, &buffer.texture);
			glBindTexture(GL_TEXTURE_2D, buffer.texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowSettings.width, windowSettings.height,
				0, GL_RGBA, GL_FLOAT, nullptr);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, buffer.texture, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Check if the framebuffer is correctly created.
			assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void PostProcessingModule::DeleteBuffers() const
	{
		delete _camBuffers;
	}

	void PostProcessingModule::SetupShader()
	{
		// Define shader.
		const GLuint vert = ShaderLoader::CreateShader(
			"Resources/passthrough.vert", GL_VERTEX_SHADER);
		const GLuint frag = ShaderLoader::CreateShader(
			"Resources/passthrough.frag", GL_FRAGMENT_SHADER);
		_program = ShaderLoader::LinkShaders(vert, frag);

		glUseProgram(_program);
		_camCount = glGetUniformLocation(_program, "camCount");

		// Assign all the uniforms.
		const GLuint cam0 = glGetUniformLocation(_program, "cameras");
		for (int32_t i = 0; i < MAX_CAMERAS; ++i)
			glUniform1i(cam0 + i, i);
	}

	void PostProcessingModule::SetupModel()
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
			GL_FALSE, sizeof(QuadVertex),
			nullptr);
		glEnableVertexAttribArray(0);

		// Texture attribute.
		glVertexAttribPointer(1, 2, GL_FLOAT,
			GL_FALSE, sizeof(QuadVertex),
			(void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);
	}
}
