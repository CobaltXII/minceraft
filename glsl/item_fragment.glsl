#version 330 core

uniform sampler2DArray the_sampler;

in vec3 frag_texture;
in float frag_lighting;

out vec4 frag_color;

void main() {
	frag_color = texture(the_sampler, frag_texture);
	if (frag_color.w == 0.0f) {
		discard;
	}
	frag_color = vec4(frag_color.xyz * frag_lighting, frag_color.w);
}