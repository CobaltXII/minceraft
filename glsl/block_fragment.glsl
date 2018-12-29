#version 330 core

uniform sampler2DArray block_texture_array;
out vec4 fragment_color;

void main()
{
	fragment_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}