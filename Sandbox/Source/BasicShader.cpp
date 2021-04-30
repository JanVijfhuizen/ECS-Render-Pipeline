#include "BasicShader.h"

#include "BakedTransform.h"
#include "glm/ext.hpp"
#include "SparseSet.h"

BasicShader::BasicShader(const GLuint program): CeShader(program)
{
	_model = GetUniformLoc("model");
}

void BasicShader::Use(const int32_t index, const glm::vec3 eye, 
	const glm::mat4& view, const glm::mat4& projection)
{
	CeShader::Use(index, eye, view, projection);

	const auto& transforms = jecs::SparseSet<BakedTransform>::Get();
	const auto& transform = transforms[index];

	// Forward the model to the shader.
	// Take note that calculating the model matrix from a transform is really expensive,
	// so normally you'd want to batch this, or calculate it in a different way.
	glUniformMatrix4fv(_model, 1, GL_FALSE, glm::value_ptr(transform.model));
}
