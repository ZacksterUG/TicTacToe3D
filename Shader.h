#pragma once
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <GLM/glm.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath, bool isGeometry);
	~Shader();

	void Bind() const;
	void UnBind() const;
	int GetID();
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, glm::vec4 value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3f(const std::string& name, glm::vec3 value);
	void SetUniform2f(const std::string& name, glm::vec2 value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniformMatrix4fv(const std::string& name, const glm::mat4& mat);
	bool UnifromExists(const std::string& name) { return glGetUniformLocation(m_RendererID, name.c_str()) != -1; }
private:
	int GetUniformLocation(const std::string& name);
	struct ShaderProgramSource ParseShader(const std::string& filepath);
	struct ShaderProgramSource ParseShaderGeometry(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
};