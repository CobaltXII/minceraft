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

// Write a vertex to a cpu_sprite.

inline void write_vertex(cpu_sprite* the_cpu_sprite, float x, float y, float xt, float yt)
{
	the_cpu_sprite->vertices[the_cpu_sprite->offset++] = x;
	the_cpu_sprite->vertices[the_cpu_sprite->offset++] = y;

	the_cpu_sprite->vertices[the_cpu_sprite->offset++] = xt;
	the_cpu_sprite->vertices[the_cpu_sprite->offset++] = yt;
}

// A structure that represents a sprite loaded to the GPU.

struct gpu_sprite
{
	GLuint sprite_vao;
	GLuint sprite_vbo;

	GLuint sprite_texture;

	unsigned int size_in_bytes;

	unsigned int size_in_floats;

	unsigned int size_in_vertices;
};

// Generate a gpu_sprite* from a cpu_sprite*.

gpu_sprite* make_gpu_sprite(cpu_sprite* the_cpu_sprite, GLuint the_texture)
{
	gpu_sprite* the_gpu_sprite = new gpu_sprite();

	the_gpu_sprite->sprite_texture = the_texture;

	the_gpu_sprite->size_in_bytes = the_cpu_sprite->size_in_bytes;

	the_gpu_sprite->size_in_floats = the_cpu_sprite->size_in_floats;

	the_gpu_sprite->size_in_vertices = the_cpu_sprite->size_in_vertices;

	GLuint the_vao;
	GLuint the_vbo;

	glGenVertexArrays(1, &the_vao);

	glGenBuffers(1, &the_vbo);

	glBindVertexArray(the_vao);

	glBindBuffer(GL_ARRAY_BUFFER, the_vbo);

	glBufferData(GL_ARRAY_BUFFER, the_cpu_sprite->size_in_bytes, the_cpu_sprite->vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	the_gpu_sprite->sprite_vao = the_vao;
	the_gpu_sprite->sprite_vbo = the_vbo;

	free(the_cpu_sprite->vertices);

	delete the_cpu_sprite;

	return the_gpu_sprite;
}
