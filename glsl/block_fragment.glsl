#version 330 core

// The default sky color.

const vec3 sky_color = vec3(186.0f / 255.0f, 214.0f / 255.0f, 254.0f / 255.0f);

// The block texture arry.

uniform sampler2DArray block_texture_array;

// The fog distance.

uniform float fog_distance;

// Input from the vertex shader.

in vec3 frag_texture;

in float frag_lighting;

in float frag_distance;

// Output to OpenGL.

out vec4 frag_color;

// Shader code.

void main()
{
	// Sample the block texture array at the given texture coordinates and set
	// the fragment color to that color.

	frag_color = texture(block_texture_array, frag_texture);

	// Reject fragments that have a w (alpha) channel of 0.0f, because those
	// fragments are fully transparent and can be ignored.

	if (frag_color.w == 0.0f)
	{
		discard;
	}

	// Multiply the fragment color by the fragment lighting.

	frag_color = vec4(frag_color.xyz * frag_lighting, frag_color.w);

	// Mix the fragment color with the sky color based on the fragment's 
	// distance to the origin over the fog's solid density distance.

	frag_color = vec4(mix(frag_color.xyz, sky_color, min(1.0f, frag_distance / fog_distance)), frag_color.w);
}