#include "ball.h"
#include <glad/gl.h>
#include <math.h>

Ball::Ball(float x, float y, float z, float radius, unsigned int numberOfSides, const idk::vec4& color): _x(x), _y(y), _z(z), _radius(radius), _numberOfSides(numberOfSides), _color(color)
{
	_model = idk::mat4();

	// Number of sides + center vertex
	int numberOfVertices = numberOfSides + 1;

	float doublePI = 2.0f * IDK_PI;

	// NOTE(Ricardo): 3 floats (pos) + 4 floats (color)
	float *vertices = new float[numberOfVertices * 7];
	for(int i=0;i<numberOfVertices * 7;i++)
	{
		vertices[i] = 0;
	}
	// Center of the circle
	vertices[0] = x;
	vertices[1] = y;
	vertices[2] = z;
	//Color
	vertices[3] = 0.0f;
	vertices[4] = 0.0f;
	vertices[5] = 1.0f;
	vertices[6] = 1.0f;


	for(unsigned int i = 1; i<numberOfVertices; i++)
	{
		float valueX = x + (radius * cos(i * doublePI/ numberOfSides));
		float valueY = y + (radius * sin(i * doublePI/ numberOfSides));
		vertices[i * 7]     = valueX;
		vertices[i * 7 + 1] = valueY;
		vertices[i * 7 + 2] = z;
		//Color RGBA
		vertices[i * 7 + 3] = i%2 ? 1.0f:0.0f;
		vertices[i * 7 + 4] = i%2 ? 0.0f:1.0f;
		vertices[i * 7 + 5] = 0.0f;
		vertices[i * 7 + 6] = 1.0f;
	}


	unsigned int *indices = new unsigned int[numberOfSides * 3];
	//Index Buffer
	for(unsigned int i = 0; i< numberOfSides; i++)
	{
		indices[i*3] = 0;
		indices[i*3+1] = i+1;
		// NOTE(Ricardo): When is the last triangle use v1 as last index
		if(i==numberOfSides - 1)
		{
			indices[i*3+2] = 1;
		}
		else
		{
			indices[i*3+2] = i+2;
		}

	}

	// Create VAO and VBO
	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glCreateBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numberOfVertices*7, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)12);

	glCreateBuffers(1,&_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*numberOfSides*3, indices, GL_STATIC_DRAW);

	_shader = std::make_unique<Shader>("assets/shaders/basic_circle.vert", "assets/shaders/basic_circle.frag");
	_shader->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Ball::Draw()
{
	_shader->Bind();

	idk::mat4 projection = idk::ortho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	_shader->SetUniformMat4("projection", projection);
	_shader->SetUniformMat4("model", _model);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

	glDrawElements(GL_TRIANGLES, _numberOfSides*3,GL_UNSIGNED_INT,0);

}

void Ball::Update()
{
	_model = idk::translate(_model, idk::vec3(0.0008f, 0.0f, 0.0f));
}
