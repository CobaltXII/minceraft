#version 330 core

layout (location = 0) in vec3 vertex_position;

layout (location = 1) in vec3 vertex_texture;

layout (location = 2) in float vertex_lighting;

uniform mat4 matrix_projection;

uniform mat4 matrix_view;

void main()
{
	gl_Position = vec4(vertex_position, 1.0f);
}