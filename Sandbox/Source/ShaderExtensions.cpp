#include "ShaderExtensions.h"

#include "BakedTransform.h"
#include "Material.h"
#include "SparseSet.h"

void ModelFwd::Init(const GLuint program)
{
	ShaderExt::Init(program);
	_model = GetUniformLoc("model");
}

void ModelFwd::Use(const int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection)
{
	const auto& transforms = jecs::SparseSet<BakedTransform>::Get();
	const auto& transform = transforms[index];

	// Forward the model to the shader.
	glUniformMatrix4fv(_model, 1, GL_FALSE, value_ptr(transform.model));
}

void MatFwd::Init(const GLuint program)
{
	ShaderExt::Init(program);
	_color = GetUniformLoc("mat.diffuse");
	_specularity = GetUniformLoc("mat.specularity");
}

void MatFwd::Use(const int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection)
{
	// Get or create.
	auto& materials = jecs::SparseSet<Material>::Get();
	const auto& material = materials.Insert(index);

	const auto& color = material.color;
	glUniform3f(_color, color.x, color.y, color.z);
	glUniform1f(_specularity, material.specularity);
}

void LightmapFwd::Init(const GLuint program)
{
	ShaderExt::Init(program);

	const GLuint diffuse = GetUniformLoc("mat.diffuseTex");
	const GLuint normal = GetUniformLoc("mat.normalTex");
	const GLuint specularity = GetUniformLoc("mat.specularityTex");

	glUniform1i(diffuse, 0);
	glUniform1i(normal, 1);
	glUniform1i(specularity, 2);
}

void LightmapFwd::Use(const int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection)
{
	// Get or create.
	auto& materials = jecs::SparseSet<Material>::Get();
	const auto& material = materials.Insert(index);
	
	// Forward textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.diffuseTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material.normalTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, material.specularityTex);
}
