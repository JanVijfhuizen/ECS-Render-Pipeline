#pragma once
#include <glm/fwd.hpp>

#include "Light.h"
#include "SparseSet.h"
#include "Shaders/ModularShader.h"

namespace rpi
{
	struct Transform;
}

// Extension that forwards lights in the scene.
class LightFwd final : public rpi::ShaderExt
{
public:
	~LightFwd();
	
	void Init(GLuint program) override;
	void Use(int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection) override;

private:
#pragma region Structs
	struct Fallof final
	{
		GLint constant = -1;
		GLint linear = -1;
		GLint quadratic = -1;
	};

	struct PointLight final
	{
		GLint diffuse = -1;
		GLint pos = -1;

		Fallof fallof{};
	};

	struct SpotLight final
	{
		GLint cutOff;

		GLint diffuse = -1;
		GLint pos = -1;
		GLint dir = -1;

		Fallof fallof{};
	};

	struct DirLight final
	{
		GLint diffuse = -1;
		GLint dir = -1;
	};

	struct Ambient final
	{
		GLint diffuse = -1;
	};

	struct Visitor final
	{
		Light* light = nullptr;
		int32_t index = -1;

		explicit Visitor(LightFwd& shader);

		void operator()(const Light::Point& point);
		void operator()(const Light::Directional& direction);
		void operator()(const Light::Spot& spot);
		void operator()(const Light::Ambient& ambient) const;

		void PostVisit() const;

	private:
		LightFwd& _shader;
		jecs::SparseSet<rpi::Transform>& _transforms;

		int32_t _ptCount = 0;
		int32_t _spotCount = 0;
		int32_t _dirCount = 0;

		void SetDiffuse(GLuint handle) const;
		void SetPosition(GLuint handle) const;
		void SetDirection(GLuint handle) const;
		static void SetFallof(const Fallof& handles, const Light::Fallof& values);
	};

	friend Visitor;
#pragma endregion

#define _MAX_LIGHT_COUNT 32
	
	Ambient _ambient{};

	GLint _ptLightCount = -1;
	PointLight* _ptLights = nullptr;
	GLint _dirLightCount = -1;
	DirLight* _dirLights = nullptr;
	GLint _spotLightCount = -1;
	SpotLight* _spotLights = nullptr;

	void SetupAmbientLight();
	void SetupPointLights();
	void SetupDirLights();
	void SetupSpotLights();
};
	