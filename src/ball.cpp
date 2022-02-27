#include "ball.h"
#include <gl.h>
#include <math.h>
#include "paddel.h"

#include "pong.h"
#include <stdlib.h>
#include <time.h>

Ball::Ball(float x, float y, float z, float radius, unsigned int numberOfSides) : _x(x), _y(y), _z(z), _radius(radius), _numberOfSides(numberOfSides)
{
}

void Ball::Init()
{
	// Number of sides + center vertex
	int numberOfVertices = _numberOfSides + 1;

	float doublePI = 2.0f * (float)IDK_PI;

	// NOTE(Ricardo): 3 floats (pos) + 4 floats (color)
	float *vertices = new float[numberOfVertices * 7];
	for (int i = 0; i < numberOfVertices * 7; i++)
	{
		vertices[i] = 0;
	}
	// Center of the circle
	vertices[0] = _x;
	vertices[1] = _y;
	vertices[2] = _z;
	// Color
	vertices[3] = 0.0f;
	vertices[4] = 0.0f;
	vertices[5] = 1.0f;
	vertices[6] = 1.0f;

	for (int i = 1; i < numberOfVertices; i++)
	{
		float valueX = _x + (_radius * cos(i * doublePI / _numberOfSides));
		float valueY = _y + (_radius * sin(i * doublePI / _numberOfSides));
		vertices[i * 7] = valueX;
		vertices[i * 7 + 1] = valueY;
		vertices[i * 7 + 2] = _z;
		// Color RGBA
		vertices[i * 7 + 3] = 1.0f;
		vertices[i * 7 + 4] = 1.0f;
		vertices[i * 7 + 5] = 1.0f;
		vertices[i * 7 + 6] = 1.0f;
	}

	unsigned int *indices = new unsigned int[_numberOfSides * 3];
	// Index Buffer
	for (unsigned int i = 0; i < _numberOfSides; i++)
	{
		indices[i * 3] = 0;
		indices[i * 3 + 1] = i + 1;
		// NOTE(Ricardo): When is the last triangle use v1 as last index
		if (i == _numberOfSides - 1)
		{
			indices[i * 3 + 2] = 1;
		}
		else
		{
			indices[i * 3 + 2] = i + 2;
		}
	}

	// Create VAO and VBO
	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glCreateBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices * 7, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)12);

	glCreateBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _numberOfSides * 3, indices, GL_STATIC_DRAW);

	_shader = std::make_unique<Shader>("assets/shaders/basic_circle.vert", "assets/shaders/basic_circle.frag");

	_shader->Bind();

	idk::mat4 projection = idk::ortho2D(-400.0f, 400.0f, -300.0f, 300.0f);
	_shader->SetUniformMat4("projection", projection);
	_shader->SetUniformVec3("size", idk::vec3(_radius, _radius, 1.0f));

	_shader->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	srand((unsigned int)time(NULL));

	double random_X = ((double)rand() / (RAND_MAX));
	double random_Y = ((double)rand() / (RAND_MAX));

	_movementSpeed = 0.0f;
	_movementDirection = idk::vec3(1.0f, 0.0f, 0.0f);
	//_movementDirection = idk::vec3(random_X, random_Y, 0.0f);
}

void Ball::Draw()
{
	_shader->Bind();
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

	glDrawElements(GL_TRIANGLES, _numberOfSides * 3, GL_UNSIGNED_INT, 0);
	_shader->Unbind();
}

void Ball::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	_shader->Bind();

	// printf("Checking: %.6f,%.6f,%.6f\n", _position.x, _position.y, _position.z);

	_shader->SetUniformVec3("position", _position);
	_shader->Unbind();
}

bool Ball::CollidedWithPad(Paddel &paddel)
{
	// Check Y
	bool bCheckY =
		_position.y < (paddel._position.y + paddel._size.y / 2.0f) &&
		_position.y > (paddel._position.y - paddel._size.y / 2.0f);

	if (bCheckY)
	{
		float radiusSquared = _radius * _radius;

		idk::vec3 ballProjectedToPaddle = {paddel._position.x + paddel._size.x * paddel._forward.x, _position.y, _position.z};
		idk::vec3 distanceFromBallToProjection = _position - ballProjectedToPaddle;

		if (distanceFromBallToProjection.SquareLength() < radiusSquared)
		{
			idk::vec3 movDir = _position - paddel._position;
			_movementDirection = idk::normalize(movDir);
			_movementDirection.z = _position.z;
		}
	}

	return false;
}

const COLLIDED Ball::CollidedWithTerrain()
{
	bool bCollidesTop = _position.y + _radius > (float)(Height / 2);
	bool bCollidesBottom = _position.y - _radius < -(float)(Height / 2);

	if (bCollidesTop || bCollidesBottom)
	{
		_movementDirection.y = -_movementDirection.y;
		return COLLIDED::NONE;
	}

	bool bCollidesRight = _position.x + _radius > (float)(Width / 2);
	bool bCollidesLeft = _position.x - _radius < -(float)(Width / 2);

	if (bCollidesRight)
		return COLLIDED::RIGHT;

	if (bCollidesLeft)
		return COLLIDED::LEFT;

	return COLLIDED::NONE;
}

void Ball::StartMovement()
{
	_movementSpeed = 350.0f;
}
