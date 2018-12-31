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

	GLuint water_target_vao;
	GLuint water_target_vbo;

	unsigned int water_target_size_in_floats;

	// When a block inside the region enclosed by a chunk changes, the chunk's
	// modified flag is set to true.

	bool modified;
};

// Create a chunk* from a subset of a world.

chunk* allocate_chunk
(
	world* input,

	unsigned int x,
	unsigned int y,
	unsigned int z,

	unsigned int x_res,
	unsigned int y_res,
	unsigned int z_res
)
{
	// Allocate memory to hold the enclosed region's vertex arrays.

	float* target = (float*)malloc(x_res * y_res * z_res * 6 * 2 * 3 * 7 * sizeof(float));

	float* water_target = (float*)malloc(x_res * y_res * z_res * 6 * 2 * 3 * 7 * sizeof(float));

	if (!target || !water_target)
	{
		std::cout << "Could not allocate enough memory for a new chunk." << std::endl;

		exit(14);
	}

	// Generate the enclosed region's vertex arrays.

	unsigned int target_size_in_floats;

	world_subset_to_mesh(input, x, y, z, x_res, y_res, z_res, target, target_size_in_floats);

	unsigned int water_target_size_in_floats;

	world_subset_to_water_mesh(input, x, y, z, x_res, y_res, z_res, water_target, water_target_size_in_floats);

	// Create the chunk*.

	chunk* the_chunk = new chunk();

	the_chunk->x = x;
	the_chunk->y = y;
	the_chunk->z = z;

	the_chunk->x_res = x_res;
	the_chunk->y_res = y_res;
	the_chunk->z_res = z_res;

	the_chunk->target_size_in_floats = target_size_in_floats;

	the_chunk->water_target_size_in_floats = water_target_size_in_floats;

	the_chunk->modified = false;

	{
		// Generate a target_vao and a target_vbo to reference the vertex data
		// of the enclosed region after it is uploaded to the GPU.

		GLuint target_vao;
		GLuint target_vbo;

		glGenVertexArrays(1, &target_vao);

		glGenBuffers(1, &target_vbo);

		// Bind the target_vao and the target_vbo to the current state.

		glBindVertexArray(target_vao);

		glBindBuffer(GL_ARRAY_BUFFER, target_vbo);

		// Upload the target vertex data to the GPU.

		glBufferData(GL_ARRAY_BUFFER, target_size_in_floats * sizeof(float), target, GL_STATIC_DRAW);

		// Free the enclosed region's vertex data from the CPU.

		free(target);

		// Enable the default vertex attributes.

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0 * sizeof(float)));

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));

		glEnableVertexAttribArray(2);

		// Unbind the target_vao and the target_vbo from the current state.

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Store the target_vao and the target_vbo in the_chunk.

		the_chunk->target_vao = target_vao;
		the_chunk->target_vbo = target_vbo;
	}

	{
		// Generate a water_target_vao and a water_target_vbo to reference 
		// the water vertex data of the enclosed region after it is uploaded
		// to the GPU.

		GLuint water_target_vao;
		GLuint water_target_vbo;

		glGenVertexArrays(1, &water_target_vao);

		glGenBuffers(1, &water_target_vbo);

		// Bind the water_target_vao and the water_target_vbo to the current 
		// state.

		glBindVertexArray(water_target_vao);

		glBindBuffer(GL_ARRAY_BUFFER, water_target_vbo);

		// Upload the water_target vertex data to the GPU.

		glBufferData(GL_ARRAY_BUFFER, water_target_size_in_floats * sizeof(float), water_target, GL_STATIC_DRAW);

		// Free the enclosed region's water vertex data from the CPU.

		free(water_target);

		// Enable the default vertex attributes.

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0 * sizeof(float)));

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));

		glEnableVertexAttribArray(2);

		// Unbind the water_target_vao and the water_target_vbo from the 
		// current state.

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Store the water_target_vao and the water_target_vbo in the_chunk.

		the_chunk->water_target_vao = water_target_vao;
		the_chunk->water_target_vbo = water_target_vbo;
	}

	// Return the_chunk.

	return the_chunk;
}

// Deallocate a chunk*.

void deallocate_chunk(chunk* to_be_annihilated)
{
	// Delete the target_vao and the target_vbo from the GPU.

	glDeleteVertexArrays(1, &to_be_annihilated->target_vao);

	glDeleteBuffers(1, &to_be_annihilated->target_vbo);

	// Delete the pointer to the chunk.

	delete to_be_annihilated;
}

// Render a chunk*.

void render_chunk(chunk* the_chunk)
{
	if (the_chunk->target_size_in_floats > 0)
	{
		// Bind the target_vao to the current state.

		glBindVertexArray(the_chunk->target_vao);

		// Draw the vertex array object as an array of triangles.

		glDrawArrays(GL_TRIANGLES, 0, the_chunk->target_size_in_floats / 7);

		// Unbind the target_vao from the current state.

		glBindVertexArray(0);
	}
}

// Render a chunk*'s water vertex array.

void render_chunk_water(chunk* the_chunk)
{
	if (the_chunk->water_target_size_in_floats > 0)
	{
		// Bind the water_target_vao to the current state.

		glBindVertexArray(the_chunk->water_target_vao);

		// Draw the water vertex array object as an array of triangles.

		glDrawArrays(GL_TRIANGLES, 0, the_chunk->water_target_size_in_floats / 7);

		// Unbind the water_target_vao from the current state.

		glBindVertexArray(0);
	}
}