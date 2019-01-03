#include <iostream>

// you must call load_sprite_preliminaries() before doing any sprite based operations.

GLuint sprite_shader_program;

void load_sprite_preliminaries()
{
	sprite_shader_program = load_program("../glsl/sprite_vertex.glsl", "../glsl/sprite_fragment.glsl");
}
// A structure that represents a sprite loaded to the CPU.

struct cpu_sprite
{
	float* vertices;

	unsigned int size_in_bytes;

	unsigned int size_in_floats;

	unsigned int size_in_vertices;

	unsigned int offset;
};
