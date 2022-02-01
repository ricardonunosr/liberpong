#pragma once

#include "IDKMath.h"
#include "shader.h"
#include "game_object.h"
#include <memory>

struct Paddel;

class Ball :public GameObject {
public:
	Ball(float x, float y, float z, float radius, unsigned int numberOfSides, const idk::vec4& color);

	void Init();

	void Draw();
	void Update(float deltaTime) override;
	bool CollidedWithPad(Paddel& paddel);
	void CollidedWithTerrain();
	void StartMovement();
public:
private:
	float _x, _y, _z, _radius;
	unsigned int _numberOfSides;
	unsigned int _vao, _vbo, _ibo;
	unsigned int _width, _height;
	const idk::vec4& _color;
	std::unique_ptr<Shader> _shader;
};
