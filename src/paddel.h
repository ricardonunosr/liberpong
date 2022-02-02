#pragma once

#include "IDKMath.h"
#include "shader.h"
#include "game_object.h"

#include <memory>

class Paddel : public GameObject
{
public:
	Paddel(const idk::vec3 &size, const idk::vec3 &position, const idk::vec3 &forward);
	Paddel() {}

	void Init();

	void Draw();
	void Update(float deltaTime) override;
	void MoveUp();
	void MoveDown();
	void Stop();

public:
	idk::vec3 _size;
	idk::vec3 _forward;

private:
	unsigned int _vao, _vbo;
	std::unique_ptr<Shader> _shader;
};
