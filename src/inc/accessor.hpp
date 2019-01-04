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

	// Set the block_id information of the voxel at the specified coordinates,
	// if the coordinates are within the bounds of the world.

	inline void set_id_safe(unsigned int x, unsigned int y, unsigned int z, block_id id)
	{
		if (!the_world->in_bounds(x, y, z))
		{
			return;
		}

		block_id old_id = the_world->get_id(x, y, z);

		if (old_id != id)
		{
			// Something actually changed.

			int cx = x / 16;
			int cy = y / 16;
			int cz = z / 16;

			the_world->set_id(x, y, z, id);

			the_chunks[cx + chunk_x_res * (cy + chunk_y_res * cz)]->modified = true;

			// If the new block_id is transparent, neighboring chunks may need
			// to be updated as well.

			if (is_transparent(id))
			{
				unsigned int xc = x % 16;
				unsigned int yc = y % 16;
				unsigned int zc = z % 16;

				// Negative X.

				if (x != 0 && xc == 0)
				{
					the_chunks[(cx - 1) + chunk_x_res * (cy + chunk_y_res * cz)]->modified = true;
				}

				// Positive X.

				if (x != the_world->x_res - 1 && xc == 15)
				{
					the_chunks[(cx + 1) + chunk_x_res * (cy + chunk_y_res * cz)]->modified = true;
				}

				// Negative Y.

				if (y != 0 && yc == 0)
				{
					the_chunks[cx + chunk_x_res * ((cy - 1) + chunk_y_res * cz)]->modified = true;
				}

				// Positive Y.

				if (y != the_world->y_res - 1 && yc == 15)
				{
					the_chunks[cx + chunk_x_res * ((cy + 1) + chunk_y_res * cz)]->modified = true;
				}

				// Negative Z.

				if (z != 0 && zc == 0)
				{
					the_chunks[cx + chunk_x_res * (cy + chunk_y_res * (cz - 1))]->modified = true;
				}

				// Positive Z.

				if (z != the_world->z_res - 1 && zc == 15)
				{
					the_chunks[cx + chunk_x_res * (cy + chunk_y_res * (cz + 1))]->modified = true;
				}
			}
		}

		if ((id == id_air) != (old_id == id_air))
 		{
 			// Update lighting.

 			propagate_skylight_strip
 			(
 				the_world, 

 				the_chunks, 

 				chunk_x_res,
 				chunk_y_res,
 				chunk_z_res,

 				x, 
 				z
 			);
 		}
	}

	// Set the block_id information of the voxel at the specified coordinates,
	// if the coordinates are within the bounds of the world, and if the 
	// current block_id of the voxel at the specified coordinates is id_air.

	inline void set_id_safe_if_air(unsigned int x, unsigned int y, unsigned int z, block_id id)
	{
		if (the_world->get_id_safe(x, y, z) == id_air)
		{
			set_id_safe(x, y, z, id);
		}
	}
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