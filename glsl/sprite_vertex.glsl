#version 330 core

// Vertex attributes.

layout (location = 0) in vec2 default_screen_position;

layout (location = 1) in vec2 default_texture_coordinates;

// Output to fragment shader.

out vec2 texture_coordinates;

// Main shader code.

void main()
{
    gl_Position = vec4(default_screen_position.x, default_screen_position.y, 0.0f, 1.0f);

    texture_coordinates = default_texture_coordinates;
}