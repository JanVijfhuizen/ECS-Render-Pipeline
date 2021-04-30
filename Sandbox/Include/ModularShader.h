#pragma once
#include "Shaders/CeShader.h"

class ModularShaderExt
{
public:
	virtual void Init(GLuint program);
	virtual void Use(int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection) = 0;

	[[nodiscard]] GLuint GetUniformLoc(const std::string& name) const;
	[[nodiscard]] GLuint GetProgram() const;

private:
	GLuint _program = 0;
};

inline void ModularShaderExt::Init(const GLuint program)
{
	_program = program;
}

inline GLuint ModularShaderExt::GetUniformLoc(const std::string& name) const
{
	return glGetUniformLocation(_program, name.c_str());
}

inline GLuint ModularShaderExt::GetProgram() const
{
	return _program;
}

template <typename ...Args>
class ModularShader : public rpi::CeShader
{
public:
	explicit ModularShader(GLuint program);
	void Use(int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection) override;

private:
#define TMPL_INDEX sizeof...(Args) - sizeof...(Tail) - 1
	
	std::tuple<Args...> _tuple{};

	template <typename Head, typename ...Tail>
	void InitExtensions();

	template <typename Head, typename ...Tail>
	void UseExtensions(int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection);
};

template <typename ... Args>
ModularShader<Args...>::ModularShader(const GLuint program): CeShader(program)
{
	InitExtensions<Args...>();
}

template <typename ... Args>
void ModularShader<Args...>::Use(const int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection)
{
	CeShader::Use(index, eye, view, projection);
	UseExtensions<Args...>(index, eye, view, projection);
}

template <typename ... Args>
template <typename Head, typename ... Tail>
void ModularShader<Args...>::InitExtensions()
{
	auto& head = std::get<TMPL_INDEX>(_tuple) = Head();
	head.Init(GetProgram());

	if constexpr (sizeof...(Tail) > 0)
		InitExtensions<Tail...>();
}

template <typename ... Args>
template <typename Head, typename ... Tail>
void ModularShader<Args...>::UseExtensions(int32_t index, glm::vec3 eye, const glm::mat4& view, const glm::mat4& projection)
{
	auto& head = std::get<TMPL_INDEX>(_tuple);
	head.Use(index, eye, view, projection);

	if constexpr (sizeof...(Tail) > 0)
		UseExtensions<Tail...>(index, eye, view, projection);
}
