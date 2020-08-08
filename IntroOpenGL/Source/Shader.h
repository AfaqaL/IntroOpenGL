#pragma once

#include<string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderSrc {
	std::string VertexSrc;
	std::string FragmentSrc;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniformMat4(const std::string& name, const glm::mat4& proj);

private:
	int GetUniformLocation(const std::string& name);
	ShaderSrc ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& str_src);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
