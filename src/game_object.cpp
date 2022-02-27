#include "game_object.h"

void GameObject::Update(float deltaTime)
{
	idk::vec3 distance = (_movementDirection * _movementSpeed) * deltaTime;
	_position += distance;
}