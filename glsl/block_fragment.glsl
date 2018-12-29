#version 330 core

uniform sampler2DArray block_texture_array;

in vec3 frag_texture;

in float frag_lighting;

out vec4 fragment_color;

void main()
{
	fragment_color = texture(block_texture_array, frag_texture);

	if (fragment_color.w == 0.0f)
	{
		discard;
	}
}