#include "PostProcessingModule.h"
#include <glad/glad.h>
#include "ShaderLoader.h"
#include <glm/vec2.hpp>

rpi::PostProcessingModule::PostProcessingModule()
{
	GenerateBuffers();
	SetupShader();
	SetupModel();
}

rpi::PostProcessingModule::~PostProcessingModule()
{
	DeleteBuffers();
	glDeleteShader(_program);
	glDeleteVertexArrays(1, &_vao);
}

void rpi::PostProcessingModule::RenderNext()
{
	assert(_currentCamIndex < MAX_CAMERAS);
	const auto& buffer = _camBuffers[_currentCamIndex++];

	glBindFramebuffer(GL_FRAMEBUFFER, buffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rpi::PostProcessingModule::PostRender()
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

void rpi::PostProcessingModule::OnWindowResize(GLFWwindow* window, 
	const int32_t width, const int32_t height)
{
	DeleteBuffers();
	GenerateBuffers();
}

rpi::PostProcessingModule::CameraBuffer::~CameraBuffer()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &texture);
}

void rpi::PostProcessingModule::GenerateBuffers()
{
	const auto& windowSettings = WindowModule::Get().GetSettings();
	_camBuffers = new CameraBuffer[MAX_CAMERAS];

	for (int32_t i = 0; i < MAX_CAMERAS; ++i)
	{
		auto& buffer = _camBuffers[i];

		glGenFramebuffers(1, &buffer.fbo);

		glGenTextures(1, &buffer.texture);
		glBindTexture(GL_TEXTURE_2D, buffer.texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowSettings.width, windowSettings.height,
			0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, buffer.fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
			GL_TEXTURE_2D, buffer.texture, 0);

		// Check if the framebuffer is correctly created.
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rpi::PostProcessingModule::DeleteBuffers() const
{
	delete _camBuffers;
}

void rpi::PostProcessingModule::SetupShader()
{
	// Define shader.
	const GLuint vert = ShaderLoader::CreateShader(
		"Resources/passthrough.vert", GL_VERTEX_SHADER);
	const GLuint frag = ShaderLoader::CreateShader(
		"Resources/passthrough.frag", GL_FRAGMENT_SHADER);
	_program = ShaderLoader::LinkShaders(vert, frag);
	_camCount = glGetUniformLocation(_program, "camCount");
}

void rpi::PostProcessingModule::SetupModel()
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
