#pragma once

#include "IDKMath.h"

#include <string>

class Shader {
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	void Bind();
	void Unbind();

	void SetUniformMat4(const std::string& name, idk::mat4 matrix);
	void SetUniformVec3(const std::string& name, const idk::vec3& vector);

private:
	const std::string ReadShaderSourceFromFile(const std::string& shaderPath);
	const unsigned int CompileShader(const std::string& shaderSource, unsigned int type);
	void LinkProgram(const unsigned int vertexShader, const unsigned int fragmentShader);

private:
	unsigned int _id;

};
