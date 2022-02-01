#include "game_object.h"

void GameObject::Update(float deltaTime)
{
	_position += (_movementDirection * _movementSpeed) * deltaTime;
}