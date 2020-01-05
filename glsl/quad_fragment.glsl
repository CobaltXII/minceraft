#version 330 core

uniform sampler2D the_sampler;

in vec2 frag_texture;

out vec4 frag_color;

void main() {
	frag_color = texture(the_sampler, frag_texture);
	if (frag_color.w == 0.0f) {
		discard;
	}
}