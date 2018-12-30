#include <iostream>

// A chunk represents a subset of the world that has been loaded on to the 
// GPU.

struct chunk
{
	unsigned int x;
	unsigned int y;
	unsigned int z;

	unsigned int x_res;
	unsigned int y_res;
	unsigned int z_res;

	GLuint target_vao;
	GLuint target_vbo;

	unsigned int target_size_in_floats;

	// When a block inside the region enclosed by a chunk changes, the chunk's
	// modified flag is set to true.

	bool modified;
};
