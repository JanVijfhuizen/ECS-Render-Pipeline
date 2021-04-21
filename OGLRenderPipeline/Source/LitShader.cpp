#include "LitShader.h"
#include "MapSet.h"
#include "Light.h"
#include "SparseSet.h"
#include "Transform.h"

namespace rpi
{
	LitShader::LitShader(const GLuint memProgram) : Shader(memProgram)
	{
		_color = GetUniformLoc("color");
		_ambient = GetUniformLoc("ambient");
	}

	void LitShader::Use(const glm::mat4& view, const glm::mat4& projection)
	{
		Shader::Use(view, projection);

		// Forward textures.
		glBindTexture(GL_TEXTURE_2D, _diffuseTexture);
		glBindTexture(GL_TEXTURE_2D, _normalTexture);
		glBindTexture(GL_TEXTURE_2D, _specularTexture);

		// Forward color.
		glUniform3f(_color, _colorVal.x, _colorVal.y, _colorVal.z);

		HandleLighting();

		// Assign lighting values.
		glUniform3f(_ambient, _ambientVal.x, _ambientVal.y, _ambientVal.z);
	}

	void LitShader::SetColor(const glm::vec3 color)
	{
		_colorVal = color;
	}

	void LitShader::SetDiffuseTex(const GLint handle)
	{
		_diffuseTexture = handle;
	}

	void LitShader::SetNormalTex(const GLint handle)
	{
		_normalTexture = handle;
	}

	void LitShader::SetSpecularTex(const GLint handle)
	{
		_specularTexture = handle;
	}

	LitShader::DefaultVisitor::DefaultVisitor(LitShader& shader) :
		_shader(shader), _transforms(jecs::SparseSet<Transform>::Get())
	{
		
	}

	void LitShader::DefaultVisitor::operator()(const Light::Point& point) const
	{
	}

	void LitShader::DefaultVisitor::operator()(const Light::Direction& direction) const
	{
	}

	void LitShader::DefaultVisitor::operator()(const Light::Ambient& ambient) const
	{
		_shader.SetAmbient(light->diffuse);
	}

	void LitShader::HandleLighting()
	{
		auto& lights = jecs::MapSet<Light>::Get();
		DefaultVisitor visitor{*this};

		for (const auto [light, index] : lights)
		{
			visitor.light = &light;
			std::visit(visitor, light.type);
		}
	}

	void LitShader::SetAmbient(const glm::vec3 value)
	{
		_ambientVal = value;
	}
}
