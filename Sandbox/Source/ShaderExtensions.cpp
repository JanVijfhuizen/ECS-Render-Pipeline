#include "ShaderExtensions.h"

#include "BakedTransform.h"
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
