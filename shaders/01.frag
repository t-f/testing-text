#version 330

in vec2 color2;
out vec4 output_color;

uniform sampler2D a;

void main()
{
	output_color = texture(a, color2);
}
