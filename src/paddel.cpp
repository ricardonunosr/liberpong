#include "paddel.h"
#include <glad/gl.h>
#include "pong.h"

#include <stdio.h>

float vertices[] = {
	// Lower triangle
	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,

	// Upper triangle
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f};

Paddel::Paddel(const idk::vec3 &size, const idk::vec3 &position, const idk::vec3 &forward) : _vao(0), _vbo(0)
{
	_position = position;
	_size = size;
	_forward = forward;
}

void Paddel::Init()
{
	// Create VAO and VBO
	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glCreateBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

	_shader = std::make_unique<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	_shader->Bind();

	idk::mat4 projection = idk::ortho2D(-400.0f, 400.0f, -300.0f, 300.0f);
	_shader->SetUniformMat4("projection", projection);
	_shader->SetUniformVec3("position", _position);
	_shader->SetUniformVec3("uColor", idk::vec3(1.0f, 1.0f, 1.0f));
	_shader->SetUniformVec3("size", _size);

	_shader->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Paddel::Draw()
{
	_shader->Bind();
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Paddel::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	_shader->Bind();
	//printf("Checking: %.6f,%.6f,%.6f\n", _position.x, _position.y, _position.z);

	// Top
	if (_position.y + _size.y / 2 >= (float)Height / 2)
	{
		Stop();
		return;
	}

	//Bottom
	if (_position.y - _size.y / 2 <= -(float)(Height / 2))
	{
		Stop();
		return;
	}

	_shader->SetUniformVec3("position", _position);
	_shader->Unbind();
}

void Paddel::MoveUp()
{
	_movementSpeed = 250.0f;
	_movementDirection = idk::vec3(0, 1.0f, 0);
}

void Paddel::MoveDown()
{
	_movementSpeed = 250.0f;
	_movementDirection = idk::vec3(0, -1.0f, 0);
}

void Paddel::Stop()
{
	_movementSpeed = 0.0f;
	_movementDirection = idk::vec3(0, 0.0f, 0);
}