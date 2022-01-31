#pragma once

#include "IDKMath.h"
#include "shader.h"

#include <memory> 

class Paddel {
public:
	Paddel(unsigned int width, unsigned int height, const idk::vec4& color);

	void Draw();
public:
	idk::mat4 _model;
private:
	unsigned int _vao, _vbo;
	unsigned int _width, _height;
	const idk::vec4& _color;
	std::unique_ptr<Shader> _shader;
};
