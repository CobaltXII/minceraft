#version 330 core

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 vertex_texture;
layout (location = 2) in vec3 vertex_lighting;

out vec2 frag_texture;
out vec3 frag_lighting;

void main() {
	gl_Position = vec4(vertex_position, 0.0f, 1.0f);
	frag_texture = vertex_texture;
	frag_lighting = vertex_lighting;
}