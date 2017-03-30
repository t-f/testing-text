#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture;
out vec2 color;

const float size = 1/16.0f;

void main()
{
	gl_Position = vec4(position, 1);
	color = vec2(texture.x+5*size, texture.y+4*size);
}
