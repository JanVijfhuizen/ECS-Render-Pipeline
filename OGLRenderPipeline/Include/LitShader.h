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

		void Use(const glm::mat4& view, const glm::mat4& projection) override;

		void SetColor(glm::vec3 color = glm::vec3{1});
		void SetDiffuseTex(GLint handle = -1);
		void SetNormalTex(GLint handle = -1);
		void SetSpecularTex(GLint handle = -1);

	protected:
		struct DefaultVisitor final
		{
			Light* light = nullptr;

			explicit DefaultVisitor(LitShader& shader);

			void operator()(const Light::Point& point) const;
			void operator()(const Light::Direction& direction) const;
			void operator()(const Light::Ambient& ambient) const;

		private:
			LitShader& _shader;
			jecs::SparseSet<Transform>& _transforms;
		};

		virtual void HandleLighting();
		void SetAmbient(glm::vec3 value);

	private:
		// Self.
		GLuint _color = -1;
		glm::vec3 _colorVal{ 1 };

		GLint _diffuseTexture = -1;
		GLint _normalTexture = -1;
		GLint _specularTexture = -1;

		// External.
		GLuint _ambient = -1;
		glm::vec3 _ambientVal{ 1 };
	};
}
