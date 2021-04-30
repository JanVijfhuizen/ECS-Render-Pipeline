#include "LightFwd.h"

#include "MapSet.h"
#include "Components/Transform.h"
#include "glm/ext.hpp"
#include "Systems/TransformSystem.h"

LightFwd::~LightFwd()
{
	delete[] _ptLights;
	delete[] _dirLights;
	delete[] _spotLights;
}

void LightFwd::Init(const GLuint program)
{
	ShaderExt::Init(program);

	_ptLights = new PointLight[_MAX_LIGHT_COUNT];
	_dirLights = new DirLight[_MAX_LIGHT_COUNT];
	_spotLights = new SpotLight[_MAX_LIGHT_COUNT];

	SetupAmbientLight();
	SetupPointLights();
	SetupDirLights();
	SetupSpotLights();
}

void LightFwd::Use(const int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection)
{
	auto& lights = jecs::MapSet<Light>::Get();
	Visitor visitor{ *this };

	for (const auto [light, index] : lights)
	{
		visitor.light = &light;
		visitor.index = index;

		std::visit(visitor, light.type);
	}

	visitor.PostVisit();
}

LightFwd::Visitor::Visitor(LightFwd& shader) : _shader(shader),
	_transforms(jecs::SparseSet<rpi::Transform>::Get())
{
		
}

void LightFwd::Visitor::operator()(const Light::Point& point)
{
	if (_ptCount >= _MAX_LIGHT_COUNT)
		return;

	const auto& ptLight = _shader._ptLights[_ptCount++];

	SetDiffuse(ptLight.diffuse);
	SetFallof(ptLight.fallof, point.fallof);
	SetPosition(ptLight.pos);
}

void LightFwd::Visitor::operator()(const Light::Directional& direction)
{
	if (_dirCount >= _MAX_LIGHT_COUNT)
		return;

	const auto& dirLight = _shader._dirLights[_dirCount++];

	SetDiffuse(dirLight.diffuse);
	SetDirection(dirLight.dir);
}

void LightFwd::Visitor::operator()(const Light::Spot& spot)
{
	if (_spotCount >= _MAX_LIGHT_COUNT)
		return;

	const auto& spotLight = _shader._spotLights[_spotCount++];
	glUniform1f(spotLight.cutOff, glm::cos(glm::radians(spot.cutOff)));

	SetDiffuse(spotLight.diffuse * 10000);
	SetPosition(spotLight.pos);
	SetDirection(spotLight.dir);
	SetFallof(spotLight.fallof, spot.fallof);
}

void LightFwd::Visitor::operator()(const Light::Ambient&) const
{
	const auto& diffuse = light->diffuse;
	glUniform3f(_shader._ambient.diffuse, diffuse.x, diffuse.y, diffuse.z);
}

void LightFwd::Visitor::PostVisit() const
{
	glUniform1i(_shader._ptLightCount, _ptCount);
	glUniform1i(_shader._dirLightCount, _dirCount);
	glUniform1i(_shader._spotLightCount, _spotCount);
}

void LightFwd::Visitor::SetDiffuse(const GLuint handle) const
{
	const auto& diffuse = light->diffuse;
	glUniform3f(handle, diffuse.x, diffuse.y, diffuse.z);
}

void LightFwd::Visitor::SetPosition(const GLuint handle) const
{
	const auto& transform = _transforms[index];
	const auto& position = transform.position;
	glUniform3f(handle, position.x, position.y, position.z);
}

void LightFwd::Visitor::SetDirection(const GLuint handle) const
{
	const auto& transform = _transforms[index];
	const auto forward = rpi::TransformSystem::GetForward(transform);
	glUniform3f(handle, forward.x, forward.y, forward.z);
}

void LightFwd::Visitor::SetFallof(const Fallof& handles, const Light::Fallof& values)
{
	glUniform1f(handles.constant, values.constant);
	glUniform1f(handles.linear, values.linear);
	glUniform1f(handles.quadratic, values.quadratic);
}

void LightFwd::SetupAmbientLight()
{
	_ambient.diffuse = GetUniformLoc("ambient");
}

void LightFwd::SetupPointLights()
{
	_ptLightCount = GetUniformLoc("ptCount");
	
	for (int32_t i = 0; i < _MAX_LIGHT_COUNT; ++i)
	{
		std::string str = "ptLights[";
		str.append(std::to_string(i));
		str.append("].");

		PointLight& light = _ptLights[i];
		light.diffuse = GetUniformLoc(str + "diffuse");
		light.pos = GetUniformLoc(str + "pos");

		auto& fallof = light.fallof;
		str += "fallof.";
		fallof.constant = GetUniformLoc(str + "constant");
		fallof.linear = GetUniformLoc(str + "linear");
		fallof.quadratic = GetUniformLoc(str + "quadratic");
	}
}

void LightFwd::SetupDirLights()
{
	_dirLightCount = GetUniformLoc("dirCount");
	
	for (int32_t i = 0; i < _MAX_LIGHT_COUNT; ++i)
	{
		std::string str = "dirLights[";
		str.append(std::to_string(i));
		str.append("].");

		DirLight& light = _dirLights[i];
		light.diffuse = GetUniformLoc(str + "diffuse");
		light.dir = GetUniformLoc(str + "dir");
	}
}

void LightFwd::SetupSpotLights()
{
	_spotLightCount = GetUniformLoc("spotCount");
	
	for (int32_t i = 0; i < _MAX_LIGHT_COUNT; ++i)
	{
		std::string str = "spotLights[";
		str.append(std::to_string(i));
		str.append("].");

		SpotLight& light = _spotLights[i];
		light.cutOff = GetUniformLoc(str + "cutOff");
		light.diffuse = GetUniformLoc(str + "diffuse");
		light.pos = GetUniformLoc(str + "pos");
		light.dir = GetUniformLoc(str + "dir");

		auto& fallof = light.fallof;
		str += "fallof.";
		fallof.constant = GetUniformLoc(str + "constant");
		fallof.linear = GetUniformLoc(str + "linear");
		fallof.quadratic = GetUniformLoc(str + "quadratic");
	}
}
