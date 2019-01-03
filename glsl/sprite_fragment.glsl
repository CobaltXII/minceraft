#version 330 core

// The sprite_texture.

uniform sampler2D sprite_texture;

// The texture_coordinates.

in vec2 texture_coordinates;

// Output to OpenGL.

out vec4 fragment_color;

// Main shader code.

void main()
{
    fragment_color = texture(sprite_texture, texture_coordinates);
}