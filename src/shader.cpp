#include "shader.h"

#include <gl.h>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
	std::string vertexShaderSource = ReadShaderSourceFromFile(vertexShaderPath);
	std::string fragShaderSource = ReadShaderSourceFromFile(fragmentShaderPath);

	const unsigned int vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
	const unsigned int fragmentShader = CompileShader(fragShaderSource, GL_FRAGMENT_SHADER);

	LinkProgram(vertexShader, fragmentShader);
	CacheUniforms();
	Unbind();
}

Shader::~Shader()
{
}

void Shader::Bind()
{
	glUseProgram(_id);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::SetUniformMat4(const std::string &name, idk::mat4 matrix)
{
	int location = GetUniformLocation(name);

	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix.columns[0].x);
}

void Shader::SetUniformVec3(const std::string &name, const idk::vec3 &vector)
{
	int location = GetUniformLocation(name);

	glUniform3f(location, vector.x, vector.y, vector.z);
}

const std::string Shader::ReadShaderSourceFromFile(const std::string &shaderPath)
{
	std::string result;
	std::ifstream filestream;

	std::string line = "";
	filestream.open(shaderPath);
	if (filestream.is_open())
	{
		while (std::getline(filestream, line))
		{
			result.append(line + "\n");
		}
		filestream.close();
	}
	else
	{
		printf("Failed to read %s! File doesn't exist.\n", shaderPath.c_str());
	}

	return result;
}

void Shader::LinkProgram(const unsigned int vertexShader, const unsigned int fragmentShader)
{
	_id = glCreateProgram();
	glAttachShader(_id, vertexShader);
	glAttachShader(_id, fragmentShader);
	glLinkProgram(_id);

	int programLinkStatus;
	glGetProgramiv(_id, GL_LINK_STATUS, &programLinkStatus);
	if (programLinkStatus != GL_TRUE)
	{
		GLsizei logLength = 0;
		char message[1024];
		glGetProgramInfoLog(_id, 1024, &logLength, message);
		std::cout << "Error Program shader: " << message << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

const unsigned int Shader::CompileShader(const std::string &shaderSource, unsigned int type)
{
	unsigned int shader = glCreateShader(type);
	const char *shaderCString = shaderSource.c_str();
	glShaderSource(shader, 1, &shaderCString, NULL);
	glCompileShader(shader);

	int shaderCompilationStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if (shaderCompilationStatus != GL_TRUE)
	{
		GLsizei logLength = 0;
		char message[1024];
		glGetShaderInfoLog(shader, 1024, &logLength, message);
		std::cout << "Error shader: " << message << std::endl;
	}

	return shader;
}

void Shader::CacheUniforms()
{
	int longestUniformMaxLenghtName;
	int numUniforms;
	glGetProgramiv(_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &longestUniformMaxLenghtName);
	glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &numUniforms);

	char *name = (char *)malloc(longestUniformMaxLenghtName * sizeof(char));
	for (int i = 0; i < numUniforms; i++)
	{
		int size;
		unsigned int type;
		glGetActiveUniform(_id, i, longestUniformMaxLenghtName, NULL, &size, &type, name);
		int location = glGetUniformLocation(_id, name);
		_uniformCache[std::string(name)] = location;
	}
}

int Shader::GetUniformLocation(const std::string &name)
{
	auto locationIter = _uniformCache.find(name);
	if (locationIter != _uniformCache.end())
	{
		auto location = locationIter->second;
		return location;
	}
	else
	{
		std::cout << "Couldn't find cached uniform:" << name << std::endl;
		return -1;
	}
}
