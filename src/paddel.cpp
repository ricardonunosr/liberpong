#include "paddel.h"
#include <glad/gl.h>


float vertices[] = {
	// Lower triangle
	-0.5f, 0.5f,0.0f,
	-0.5f,-0.5f,0.0f,
	0.5f,-0.5f,0.0f,

	// Upper triangle
	-0.5f, 0.5f,0.0f,
	0.5f, 0.5f,0.0f,
	0.5f,-0.5f,0.0f
};


Paddel::Paddel(unsigned int width, unsigned int height, const idk::vec4& color) :_width(width), _height(height), _color(color)
{
	_model = idk::mat4();
	// Create VAO and VBO
	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glCreateBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	_shader = std::make_unique<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	_shader->Bind();
	_shader->SetUniformVec3("uColor",idk::vec3(1.0f,0.0f,0.0f));
	_shader->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Paddel::Draw()
{
	_shader->Bind();

	idk::mat4 projection = idk::ortho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	_shader->SetUniformMat4("projection", projection);
	_shader->SetUniformMat4("model", _model);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

