#version 330 core

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec3 vertex_texture;
layout (location = 2) in float vertex_lighting;

out vec3 frag_texture;
out float frag_lighting;

uniform float time_in_seconds;

void main() {
	gl_Position = vec4(vertex_position, 0.0f, 1.0f);
	frag_texture = vertex_texture;
	if (frag_texture.z < 0.0f) {
		frag_texture.z = abs(frag_texture.z) + mod(floor(time_in_seconds * 16.0f), 31.0f);
	}
	frag_lighting = vertex_lighting;
}