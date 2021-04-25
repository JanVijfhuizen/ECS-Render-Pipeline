#include "LitShader.h"
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
		_spotCount = GetUniformLoc("spotCount");
		_dirCount = GetUniformLoc("dirCount");

		SetupDirLights();
		SetupSpotLights();
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
		glUniform1i(_spotCount, _spotCountVal);
		glUniform1i(_dirCount, _dirCountVal);
	}

	void LitShader::SetColor(const glm::vec3 color)
	{
		_colorVal = color;
	}

	void LitShader::SetSpecularity(const int32_t val)
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
		
		SetDiffuse(ptLight.diffuse);
		SetFallof(ptLight.fallof, point.fallof);
		SetPosition(ptLight.pos);
	}

	void LitShader::DefaultVisitor::operator()(const Light::Directional&)
	{
		if (_dirCount >= _shader._maxDirLights)
			return;

		const auto& dirLight = _shader._dirLights[_dirCount++];
		
		SetDiffuse(dirLight.diffuse);
		SetDirection(dirLight.dir);
	}

	void LitShader::DefaultVisitor::operator()(const Light::Spot& spot)
	{
		if (_spotCount >= _shader._maxSpotLights)
			return;

		const auto& spotLight = _shader._spotLights[_spotCount++];
		glUniform1f(spotLight.cutOff, glm::cos(glm::radians(spot.cutOff)));
		
		SetDiffuse(spotLight.diffuse * 10000);
		SetPosition(spotLight.pos);
		SetDirection(spotLight.dir);
		SetFallof(spotLight.fallof, spot.fallof);
	}

	void LitShader::DefaultVisitor::operator()(const Light::Ambient&) const
	{
		_shader.SetAmbient(light->diffuse);
	}

	void LitShader::DefaultVisitor::PostVisit() const
	{
		_shader.SetPointLightCount(_ptCount);
		_shader.SetSpotLightCount(_spotCount);
		_shader.SetDirLightCount(_dirCount);
	}

	void LitShader::DefaultVisitor::SetDiffuse(const GLuint handle) const
	{
		const auto& diffuse = light->diffuse;
		glUniform3f(handle, diffuse.x, diffuse.y, diffuse.z);
	}

	void LitShader::DefaultVisitor::SetPosition(const GLuint handle) const
	{
		const auto& transform = _transforms[index];
		const auto& position = transform.position;
		glUniform3f(handle, position.x, position.y, position.z);
	}

	void LitShader::DefaultVisitor::SetDirection(const GLuint handle) const
	{
		const auto& transform = _transforms[index];
		const auto forward = TransformSystem::GetForward(transform);
		glUniform3f(handle, forward.x, forward.y, forward.z);
	}

	void LitShader::DefaultVisitor::SetFallof(const Fallof& handles, const Light::Fallof& values)
	{
		glUniform1f(handles.constant, values.constant);
		glUniform1f(handles.linear, values.linear);
		glUniform1f(handles.quadratic, values.quadratic);
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

	void LitShader::SetMaxSpotLights(const int32_t num)
	{
		_maxSpotLights = num;
		SetupSpotLights();
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

	void LitShader::SetSpotLightCount(const int32_t count)
	{
		_spotCountVal = count;
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

			auto& fallof = light.fallof;
			fallof.constant = GetUniformLoc(str + "constant");
			fallof.linear = GetUniformLoc(str + "linear");
			fallof.quadratic = GetUniformLoc(str + "quadratic");

			_ptLights.push_back(light);
		}
	}

	void LitShader::SetupSpotLights()
	{
		_spotLights.clear();

		for (int32_t i = 0; i < _maxSpotLights; ++i)
		{
			std::string str = "spotLights[";
			str.append(std::to_string(i));
			str.append("].");

			SpotLight light;
			light.cutOff = GetUniformLoc(str + "cutOff");
			light.diffuse = GetUniformLoc(str + "diffuse");
			light.pos = GetUniformLoc(str + "pos");
			light.dir = GetUniformLoc(str + "dir");

			auto& fallof = light.fallof;
			fallof.constant = GetUniformLoc(str + "constant");
			fallof.linear = GetUniformLoc(str + "linear");
			fallof.quadratic = GetUniformLoc(str + "quadratic");

			_spotLights.push_back(light);
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
