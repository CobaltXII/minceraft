#include <vector>
#include <tuple>

// Propagate skylight throughout an entire world.

void propagate_skylight(world* out)
{
	// For each vertical strip of land, start with a lighting value of 15. Go
	// downwards, if the block is air, stay at light level 15. If not, set 
	// everything below it to 0.

	for (int x = 0; x < out->x_res; x++)
	{
		for (int z = 0; z < out->z_res; z++)
		{
			unsigned char natural = 15;

			for (int y = 0; y < out->y_res; y++)
			{
				out->set_natural(x, y, z, natural);

				if (out->get_id(x, y, z) != id_air)
				{
					natural = 0;
				}
			}
		}
	}

	// All blocks that are of light level 0 that have one or more neighbors 
	// that have a light level of 15 are added to the light_queue.

	std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> light_queue;

	for (unsigned int x = 0; x < out->x_res; x++)
	for (unsigned int y = 0; y < out->y_res; y++)
	for (unsigned int z = 0; z < out->z_res; z++)
	{
		if (out->get_natural(x, y, z) == 0)
		{
			if 
			(
				out->get_natural_safe(x + 1, y, z) == 15 ||
				out->get_natural_safe(x - 1, y, z) == 15 ||

				out->get_natural_safe(x, y + 1, z) == 15 ||
				out->get_natural_safe(x, y - 1, z) == 15 ||

				out->get_natural_safe(x, y, z + 1) == 15 ||
				out->get_natural_safe(x, y, z - 1) == 15
			)
			{
				out->set_natural(x, y, z, 14);

				light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y, z));
			}
		}
	}

	// Pop the top off of the queue. We'll call this the 'current block'. 
	// Check it's six neighbors, if any of them have a light value less than 
	// the current block's light value minus one, set that neighbors light 
	// value to the current block's light value minus one, and add that 
	// neighbor to the queue. Keep doing this until the queue is empty.

	while (light_queue.size() > 0)
	{
		int x = std::get<0>(light_queue[light_queue.size() - 1]);
		int y = std::get<1>(light_queue[light_queue.size() - 1]);
		int z = std::get<2>(light_queue[light_queue.size() - 1]);

		light_queue.pop_back();

		unsigned char current_value = out->get_natural(x, y, z);

		// Right neighbor.

		if (out->in_bounds(x + 1, y, z) && out->get_natural(x + 1, y, z) < current_value - 1)
		{
			out->set_natural(x + 1, y, z, current_value - 1);

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x + 1, y, z));
		}

		// Left neighbor.

		if (out->in_bounds(x - 1, y, z) && out->get_natural(x - 1, y, z) < current_value - 1)
		{
			out->set_natural(x - 1, y, z, current_value - 1);

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x - 1, y, z));
		}

		// Bottom neighbor.

		if (out->in_bounds(x, y + 1, z) && out->get_natural(x, y + 1, z) < current_value - 1)
		{
			out->set_natural(x, y + 1, z, current_value - 1);

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y + 1, z));
		}

		// Top neighbor.

		if (out->in_bounds(x, y - 1, z) && out->get_natural(x, y - 1, z) < current_value - 1)
		{
			out->set_natural(x, y - 1, z, current_value - 1);

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y - 1, z));
		}

		// Front neighbor.

		if (out->in_bounds(x, y, z + 1) && out->get_natural(x, y, z + 1) < current_value - 1)
		{
			out->set_natural(x, y, z + 1, current_value - 1);

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y, z + 1));
		}

		// Back neighbor.

		if (out->in_bounds(x, y, z - 1) && out->get_natural(x, y, z - 1) < current_value - 1)
		{
			out->set_natural(x, y, z - 1, current_value - 1);

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y, z - 1));
		}
	}
}

// Propagate skylight throughout a vertical strip of a world. Set the modified
// flag of every accessed chunk to true.

void propagate_skylight_strip
(
	world* the_world,

	chunk**& the_chunks,

	unsigned int chunk_x_res,
	unsigned int chunk_y_res,
	unsigned int chunk_z_res,

	int cx,
	int cz
)
{
	int fx = cx + 1;
	int fz = cz + 1;

	// For each vertical strip of land, start with a lighting value of 15. Go
	// downwards, if the block is air, stay at light level 15. If not, set 
	// everything below it to 0. Add all X and Z neighbors to the queue.

	std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> light_queue;

	for (int x = cx; x < fx; x++)
	{
		for (int z = cz; z < fz; z++)
		{
			unsigned char natural = 15;

			for (int y = 0; y < the_world->y_res; y++)
			{
				the_world->set_natural(x, y, z, natural);

				the_chunks[(x / 16) + chunk_x_res * ((y / 16) + chunk_y_res * (z / 16))]->modified = true;

				if (the_world->get_id(x, y, z) != id_air)
				{
					natural = 0;
				}

				light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y, z));

				light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x - 1, y, z));
				light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x + 1, y, z));

				light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y, z - 1));
				light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y, z + 1));
			}
		}
	}

	// Pop the top off of the queue. We'll call this the 'current block'. 
	// Check it's six neighbors, if any of them have a light value less than 
	// the current block's light value minus one, set that neighbors light 
	// value to the current block's light value minus one, and add that 
	// neighbor to the queue. Keep doing this until the queue is empty.

	while (light_queue.size() > 0)
	{
		int x = std::get<0>(light_queue[light_queue.size() - 1]);
		int y = std::get<1>(light_queue[light_queue.size() - 1]);
		int z = std::get<2>(light_queue[light_queue.size() - 1]);

		light_queue.pop_back();

		unsigned char current_value = the_world->get_natural(x, y, z);

		// Right neighbor.

		if (the_world->in_bounds(x + 1, y, z) && the_world->get_natural(x + 1, y, z) < current_value - 1)
		{
			the_world->set_natural(x + 1, y, z, current_value - 1);

			the_chunks[((x + 1) / 16) + chunk_x_res * ((y / 16) + chunk_y_res * (z / 16))]->modified = true;

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x + 1, y, z));
		}

		// Left neighbor.

		if (the_world->in_bounds(x - 1, y, z) && the_world->get_natural(x - 1, y, z) < current_value - 1)
		{
			the_world->set_natural(x - 1, y, z, current_value - 1);

			the_chunks[((x - 1) / 16) + chunk_x_res * ((y / 16) + chunk_y_res * (z / 16))]->modified = true;

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x - 1, y, z));
		}

		// Bottom neighbor.

		if (the_world->in_bounds(x, y + 1, z) && the_world->get_natural(x, y + 1, z) < current_value - 1)
		{
			the_world->set_natural(x, y + 1, z, current_value - 1);

			the_chunks[(x / 16) + chunk_x_res * (((y + 1) / 16) + chunk_y_res * (z / 16))]->modified = true;

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y + 1, z));
		}

		// Top neighbor.

		if (the_world->in_bounds(x, y - 1, z) && the_world->get_natural(x, y - 1, z) < current_value - 1)
		{
			the_world->set_natural(x, y - 1, z, current_value - 1);

			the_chunks[(x / 16) + chunk_x_res * (((y - 1) / 16) + chunk_y_res * (z / 16))]->modified = true;

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y - 1, z));
		}

		// Front neighbor.

		if (the_world->in_bounds(x, y, z + 1) && the_world->get_natural(x, y, z + 1) < current_value - 1)
		{
			the_world->set_natural(x, y, z + 1, current_value - 1);

			the_chunks[(x / 16) + chunk_x_res * ((y / 16) + chunk_y_res * ((z + 1) / 16))]->modified = true;

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y, z + 1));
		}

		// Back neighbor.

		if (the_world->in_bounds(x, y, z - 1) && the_world->get_natural(x, y, z - 1) < current_value - 1)
		{
			the_world->set_natural(x, y, z - 1, current_value - 1);

			the_chunks[(x / 16) + chunk_x_res * ((y / 16) + chunk_y_res * ((z - 1) / 16))]->modified = true;

			light_queue.push_back(std::tuple<unsigned int, unsigned int, unsigned int>(x, y, z - 1));
		}
	}
}