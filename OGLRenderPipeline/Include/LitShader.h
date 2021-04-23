#pragma once
#include "Shader.h"
#include "SparseSet.h"
#include "Transform.h"
#include "Light.h"

namespace rpi
{
	class LitShader : public Shader
	{
	public:
		explicit LitShader(GLuint memProgram);

		void Use(glm::vec3 camPos, const glm::mat4& view, const glm::mat4& projection) override;

		void SetColor(glm::vec3 color = glm::vec3{1});
		void SetSpecularity(int32_t val);

		void SetDiffuseTex(GLint handle = -1);
		void SetNormalTex(GLint handle = -1);
		void SetSpecularTex(GLint handle = -1);

	protected:
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

			Fallof fallof;
		};

		struct SpotLight final
		{
			GLuint cutOff;
			
			GLint diffuse = -1;
			GLint pos = -1;
			GLint dir = -1;
			
			Fallof fallof;
		};

		struct DirLight final
		{
			GLint diffuse = -1;
			GLint dir = -1;
		};

		struct DefaultVisitor final
		{
			Light* light = nullptr;
			int32_t index = -1;

			explicit DefaultVisitor(LitShader& shader);

			void operator()(const Light::Point& point);
			void operator()(const Light::Directional& direction);
			void operator()(const Light::Spot& spot);
			void operator()(const Light::Ambient& ambient) const;

			void PostVisit() const;

		private:
			LitShader& _shader;
			jecs::SparseSet<Transform>& _transforms;

			int32_t _ptCount = 0;
			int32_t _spotCount = 0;
			int32_t _dirCount = 0;

			void SetDiffuse(GLuint handle) const;
			void SetPosition(GLuint handle) const;
			void SetDirection(GLuint handle) const;
			static void SetFallof(const Fallof& handles, const Light::Fallof& values);
		};

		virtual void HandleLighting();

		[[nodiscard]] int32_t GetMaxPointLights() const;
		[[nodiscard]] int32_t GetMaxDirLights() const;

		void SetMaxPointLights(int32_t num);
		void SetMaxSpotLights(int32_t num);
		void SetMaxDirLights(int32_t num);

		void SetAmbient(glm::vec3 value = {});
		
		void SetPointLightCount(int32_t count = 0);
		void SetSpotLightCount(int32_t count = 0);
		void SetDirLightCount(int32_t count = 0);

	private:
		GLuint _viewPos = 0;

		// Material.
		GLuint _color = -1;
		glm::vec3 _colorVal{ 1 };

		GLuint _specularity = -1;
		int32_t _specularityVal = 32;

		GLint _diffuseTexture = -1;
		GLint _normalTexture = -1;
		GLint _specularTexture = -1;

		// Lighting.
		std::vector<PointLight> _ptLights{};
		std::vector<SpotLight> _spotLights{};
		std::vector<DirLight> _dirLights{};

		int32_t _maxDirLights = 4;
		int32_t _maxSpotLights = 4;
		int32_t _maxPointLights = 4;

		GLuint _ambient = -1;
		glm::vec3 _ambientVal{ 1 };

		GLuint _ptCount = -1;
		int32_t _ptCountVal = 0;

		GLuint _spotCount = -1;
		int32_t _spotCountVal = 0;
		
		GLuint _dirCount = -1;
		int32_t _dirCountVal = 0;

		void SetupPointLights();
		void SetupSpotLights();
		void SetupDirLights();
	};
}
