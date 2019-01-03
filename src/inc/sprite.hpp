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

// Allocate a chunk of memory (on the CPU) that can hold vertex_count vertices
// with 2-dimensional coordinates and 2-dimensional texture coordinates.

cpu_sprite* allocate_sprite(unsigned int vertex_count)
{
	float* memory = (float*)malloc(vertex_count * 4 * sizeof(float));

	if (!memory)
	{
		std::cout << "Could not allocate a vertex array for " << vertex_count << " vertices." << std::endl;

		exit(20);
	}

	cpu_sprite* the_cpu_sprite = new cpu_sprite();

	the_cpu_sprite->vertices = memory;

	the_cpu_sprite->size_in_bytes = vertex_count * 4 * sizeof(float);

	the_cpu_sprite->size_in_floats = vertex_count * 4;

	the_cpu_sprite->size_in_vertices = vertex_count;

	the_cpu_sprite->offset = 0;

	return the_cpu_sprite;
}
