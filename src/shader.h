#pragma once

#include "IDKMath.h"

#include <string>

class Shader {
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	void Bind();
	void Unbind();

	void SetUniformMat4(const std::string& name, mat4 matrix);

private:
	const std::string ReadShaderSourceFromFile(const std::string& shaderPath);
	const unsigned int CompileShader(const std::string& shaderSource, unsigned int type);
	void LinkProgram(const unsigned int vertexShader, const unsigned int fragmentShader);

private:
	unsigned int _id;

};