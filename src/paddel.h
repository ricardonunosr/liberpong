#pragma once

#include "IDKMath.h"
#include "shader.h"

#include <memory> 

class Paddel {
public:
	Paddel(unsigned int width, unsigned int height, const vec4& color);

	void Draw();

private:
	unsigned int _vao, _vbo;
	unsigned int _width, _height;
	const vec4& _color;
	std::unique_ptr<Shader> _shader;
};