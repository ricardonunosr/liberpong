#pragma once

#include "IDKMath.h"

class GameObject {
public:
	virtual void Update(float deltaTime);
public:
	idk::vec3 _position;
	float _movementSpeed = 0;
	idk::vec3 _movementDirection;
};