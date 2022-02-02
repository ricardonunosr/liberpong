#pragma once

#include "IDKMath.h"
#include "shader.h"
#include "game_object.h"
#include <memory>

struct Paddel;

enum COLLIDED
{
	NONE = 0,
	LEFT = 1,
	RIGHT = 2
};

class Ball : public GameObject
{
public:
	Ball(float x, float y, float z, float radius, unsigned int numberOfSides);
	Ball() {}

	void Init();

	void Draw();
	void Update(float deltaTime) override;
	bool CollidedWithPad(Paddel &paddel);
	const COLLIDED CollidedWithTerrain();
	void StartMovement();

public:
private:
	float _x, _y, _z, _radius;
	unsigned int _numberOfSides;
	unsigned int _vao, _vbo, _ibo;
	unsigned int _width, _height;
	std::unique_ptr<Shader> _shader;
};
