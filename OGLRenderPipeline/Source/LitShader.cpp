#include "LitShader.h"


#include <iostream>
#include <glm/trigonometric.hpp>

#include "MapSet.h"
#include "Light.h"
#include "SparseSet.h"
#include "Transform.h"
#include "TransformSystem.h"

namespace rpi
{
	LitShader::LitShader(const GLuint memProgram) : Shader(memProgram)
	{
		_viewPos = GetUniformLoc("viewPos");
		_color = GetUniformLoc("mat.color");
		_specularity = GetUniformLoc("mat.specularity");
		_ambient = GetUniformLoc("ambient");

		_ptCount = GetUniformLoc("ptCount");
		_dirCount = GetUniformLoc("dirCount");

		SetupDirLights();
		SetupPointLights();
	}

	void LitShader::Use(const glm::vec3 camPos, const glm::mat4& view, const glm::mat4& projection)
	{
		Shader::Use(camPos, view, projection);

		// Forward textures.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _diffuseTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _normalTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _specularTexture);

		glUniform1i(_diffuseTexture, 0);
		glUniform1i(_normalTexture, 1);
		glUniform1i(_specularTexture, 2);

		// Forward camera position.
		glUniform3f(_viewPos, camPos.x, camPos.y, camPos.z);

		// Forward material.
		glUniform3f(_color, _colorVal.x, _colorVal.y, _colorVal.z);
		glUniform1i(_specularity, _specularityVal);

		HandleLighting();

		// Assign lighting values.
		glUniform3f(_ambient, _ambientVal.x, _ambientVal.y, _ambientVal.z);

		// Forward the amount of lights.
		glUniform1i(_ptCount, _ptCountVal);
		glUniform1i(_dirCount, _dirCountVal);
	}

	void LitShader::SetColor(const glm::vec3 color)
	{
		_colorVal = color;
	}

	void LitShader::SetSpecularity(const float val)
	{
		_specularity = val;
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

	void LitShader::DefaultVisitor::operator()(const Light::Point& point)
	{
		if (_ptCount >= _shader._maxPointLights)
			return;

		const auto& ptLight = _shader._ptLights[_ptCount++];
		const auto& transform = _transforms[index];

		// Set diffuse.
		const auto& diffuse = light->diffuse;
		glUniform3f(ptLight.diffuse, diffuse.x, diffuse.y, diffuse.z);

		glUniform1f(ptLight.constant, point.constant);
		glUniform1f(ptLight.linear, point.linear);
		glUniform1f(ptLight.quadratic, point.quadratic);

		// Set position.
		const auto& position = transform.position;
		glUniform3f(ptLight.pos, position.x, position.y, position.z);
	}

	void LitShader::DefaultVisitor::operator()(const Light::Directional& direction)
	{
		if (_dirCount >= _shader._maxDirLights)
			return;

		const auto& dirLight = _shader._dirLights[_dirCount++];

		// Set diffuse.
		const auto& diffuse = light->diffuse;
		glUniform3f(dirLight.diffuse, diffuse.x, diffuse.y, diffuse.z);

		// Set direction.
		const auto& transform = _transforms[index];
		const auto forward = TransformSystem::GetForward(transform);
		
		glUniform3f(dirLight.dir, forward.x, forward.y, forward.z);
	}

	void LitShader::DefaultVisitor::operator()(const Light::Ambient& ambient) const
	{
		_shader.SetAmbient(light->diffuse);
	}

	void LitShader::DefaultVisitor::PostVisit() const
	{
		_shader.SetPointLightCount(_ptCount);
		_shader.SetDirLightCount(_dirCount);
	}

	void LitShader::HandleLighting()
	{
		SetAmbient();

		auto& lights = jecs::MapSet<Light>::Get();
		DefaultVisitor visitor{*this};

		for (const auto [light, index] : lights)
		{
			visitor.light = &light;
			visitor.index = index;

			std::visit(visitor, light.type);
		}

		visitor.PostVisit();
	}

	int32_t LitShader::GetMaxPointLights() const
	{
		return _maxPointLights;
	}

	int32_t LitShader::GetMaxDirLights() const
	{
		return _maxDirLights;
	}

	void LitShader::SetMaxPointLights(const int32_t num)
	{
		_maxPointLights = num;
		SetupPointLights();
	}

	void LitShader::SetMaxDirLights(const int32_t num)
	{
		_maxPointLights = num;
		SetupDirLights();
	}

	void LitShader::SetAmbient(const glm::vec3 value)
	{
		_ambientVal = value;
	}

	void LitShader::SetPointLightCount(const int32_t count)
	{
		_ptCountVal = count;
	}

	void LitShader::SetDirLightCount(const int32_t count)
	{
		_dirCountVal = count;
	}

	void LitShader::SetupPointLights()
	{
		_ptLights.clear();

		for (int32_t i = 0; i < _maxPointLights; ++i)
		{
			std::string str = "ptLights[";
			str.append(std::to_string(i));
			str.append("].");

			PointLight light;
			light.diffuse = GetUniformLoc(str + "diffuse");
			light.pos = GetUniformLoc(str + "pos");

			light.constant = GetUniformLoc(str + "constant");
			light.linear = GetUniformLoc(str + "linear");
			light.quadratic = GetUniformLoc(str + "quadratic");

			_ptLights.push_back(light);
		}
	}

	void LitShader::SetupDirLights()
	{
		_dirLights.clear();

		for (int32_t i = 0; i < _maxDirLights; ++i)
		{
			std::string str = "dirLights[";
			str.append(std::to_string(i));
			str.append("].");

			DirLight light;
			light.diffuse = GetUniformLoc(str + "diffuse");
			light.dir = GetUniformLoc(str + "dir");
			_dirLights.push_back(light);
		}
	}
}
