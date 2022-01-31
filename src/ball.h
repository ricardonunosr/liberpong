#pragma once
#include "IDKMath.h"
#include "shader.h"
#include <memory>

class Ball{
public:
	Ball(float x, float y, float z, float radius, unsigned int numberOfSides, const idk::vec4& color);

	void Draw();
	void Update();
public:
	idk::mat4 _model;
private:
	float _x, _y, _z, _radius;
	unsigned int _numberOfSides;
	unsigned int _vao, _vbo, _ibo;
	unsigned int _width, _height;
	const idk::vec4& _color;
	std::unique_ptr<Shader> _shader;
};
