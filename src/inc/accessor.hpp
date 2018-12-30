// An accessor struct acts as a middleman between common code and world 
// objects. It's job is to queue up all the changes made to a world, and
// then regenerate all required resources with a single function call.

struct accessor
{
	world* the_world;

	chunk** the_chunks;

	unsigned int chunk_x_res;
	unsigned int chunk_y_res;
	unsigned int chunk_z_res;

	unsigned int chunk_count;
};

// Create an accessor* from a world*.

accessor* allocate_accessor(world* the_world)
{
	// Create the accessor*.

	accessor* the_accessor = new accessor();

	the_accessor->the_world = the_world;

	the_accessor->chunk_x_res = the_world->x_res / 16;
	the_accessor->chunk_y_res = the_world->y_res / 16;
	the_accessor->chunk_z_res = the_world->z_res / 16;

	the_accessor->chunk_count = the_accessor->chunk_x_res * the_accessor->chunk_y_res * the_accessor->chunk_z_res;

	// Allocate an array of chunk pointers.

	the_accessor->the_chunks = (chunk**)malloc(the_accessor->chunk_count * sizeof(chunk*));

	if (!the_accessor->the_chunks)
	{
		std::cout << "Could not allocate enough memory for a new accessor." << std::endl;

		exit(15);
	}

	// Generate all of the chunks.

	for (int x = 0; x < the_accessor->chunk_x_res; x++)
	{
		for (int y = 0; y < the_accessor->chunk_y_res; y++)
		{
			for (int z = 0; z < the_accessor->chunk_z_res; z++)
			{
				the_accessor->the_chunks[x + the_accessor->chunk_x_res * (y + the_accessor->chunk_y_res * z)] = allocate_chunk(the_world, x * 16, y * 16, z * 16, 16, 16, 16);
			}
		}
	}

	// Return the accessor*.

	return the_accessor;
}

// Deallocate an accessor*.

void deallocate_accessor(accessor* the_accessor)
{
	// Destroy all of the contained chunks.

	for (int i = 0; i < the_accessor->chunk_count; i++)
	{
		deallocate_chunk(the_accessor->the_chunks[i]);
	}

	// Delete the accessor*.

	delete the_accessor;
}