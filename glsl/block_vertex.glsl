#version 330 core

layout (location = 0) in vec3 vertex_position;

layout (location = 1) in vec3 vertex_texture;

layout (location = 2) in float vertex_lighting;

uniform mat4 matrix_projection;

uniform mat4 matrix_view;

out vec3 frag_texture;

out float frag_lighting;

void main()
{
	gl_Position = matrix_projection * matrix_view * vec4(vertex_position, 1.0f);

	frag_texture = vertex_texture;

	frag_lighting = vertex_lighting;
}