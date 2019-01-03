// A structure that represents a sprite loaded to the CPU.

struct cpu_sprite
{
	float* vertices;

	unsigned int size_in_bytes;

	unsigned int size_in_floats;

	unsigned int size_in_vertices;

	unsigned int offset;
};
