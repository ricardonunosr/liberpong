#version 330 core
layout(location=0) in vec3 aPos;

uniform mat4 projection;
uniform vec3 position;
uniform vec3 size;

void main()
{
	vec3 pos = (aPos * size) + position;
	gl_Position = projection * vec4(pos,1.0);
}
