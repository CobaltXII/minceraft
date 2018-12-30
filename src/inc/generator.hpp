#include <tuple>
#include <vector>

// Generate a world using a seed.

void generate_world(world* out, unsigned int seed)
{
	// Use rand() and RAND_MAX for white noise.

	srand(seed);

	// Use FastNoise for fractal Simplex noise.

	FastNoise noise;

	noise.SetSeed(seed);

	noise.SetNoiseType(FastNoise::SimplexFractal);

	noise.SetFractalOctaves(4);

	// A small easter egg: sometimes mushroom worlds will spawn!

	bool mushroom_world = rand() % 1024 == 0;

	// Generate the base terrain.

	float frequency = 4.0f;

	for (float x = 0.0f; x < float(out->x_res); x += 1.0f)
	for (float y = 0.0f; y < float(out->y_res); y += 1.0f)
	for (float z = 0.0f; z < float(out->z_res); z += 1.0f)
	{
		if (y / float(out->y_res) + noise.GetValueFractal(x * frequency, y * frequency, z * frequency) * 0.32f > 0.5f)
		{
			out->set_id(int(x), int(y), int(z), id_stone);
		}
	}

	// Set the highground blocks.

	for (float x = 0.0f; x < float(out->x_res); x += 1.0f)
	for (float z = 0.0f; z < float(out->z_res); z += 1.0f)
	for (float y = 0.0f; y < float(out->y_res); y += 1.0f)
	{
		if (out->get_id(int(x), int(y), int(z)) != id_air)
		{
			if (y / float(out->y_res) > 0.6f)
			{
				if (noise.GetValueFractal(x * frequency, y * frequency) >= -0.2f)
				{
					out->set_id_safe(int(x), int(y), int(z), id_sand);

					out->set_id_safe_if_not_air(int(x), int(y) + 1, int(z), id_sandstone);
					out->set_id_safe_if_not_air(int(x), int(y) + 2, int(z), id_sandstone);
					out->set_id_safe_if_not_air(int(x), int(y) + 3, int(z), id_sandstone);
				}
				else
				{
					out->set_id_safe(int(x), int(y), int(z), id_gravel);

					out->set_id_safe_if_not_air(int(x), int(y) + 1, int(z), id_gravel);
					out->set_id_safe_if_not_air(int(x), int(y) + 2, int(z), id_gravel);
					out->set_id_safe_if_not_air(int(x), int(y) + 3, int(z), id_gravel);
				}
			}
			else
			{
				if (mushroom_world)
				{
					out->set_id_safe(int(x), int(y), int(z), id_mycelium);
				}
				else
				{
					out->set_id_safe(int(x), int(y), int(z), id_grass);
				}

				out->set_id_safe_if_not_air(int(x), int(y) + 1, int(z), id_dirt);
				out->set_id_safe_if_not_air(int(x), int(y) + 2, int(z), id_dirt);
				out->set_id_safe_if_not_air(int(x), int(y) + 3, int(z), id_dirt);
			}

			break;
		}
	}

	// Add water and beaches.

	for (float x = 0.0f; x < float(out->x_res); x += 1.0f)
	for (float y = 0.0f; y < float(out->y_res); y += 1.0f)
	for (float z = 0.0f; z < float(out->z_res); z += 1.0f)
	{
		if (y / float(out->y_res) > 0.5f)
		{
			if (out->get_id(int(x), int(y), int(z)) == id_air)
			{
				out->set_id(int(x), int(y), int(z), id_water);
			}
			else if (out->get_id(int(x), int(y), int(z)) == id_grass || out->get_id(int(x), int(y), int(z)) == id_mycelium)
			{
				out->set_id(int(x), int(y), int(z), id_sand);
			}
		}
	}

	// Plant trees.

	if (mushroom_world)
	{
		goto done_trees;
	}

	for (int i = 0; i < out->x_res * out->z_res / 32; i++)
	{
		tree:

		int x = rand() % out->x_res;
		int z = rand() % out->z_res;

		block_id tree_leaf;

		block_id tree_log;

		if (rand() % 3 == 0)
		{
			tree_leaf = id_birch_leaves;

			tree_log = id_birch_log;
		}
		else
		{
			tree_leaf = id_oak_leaves;

			tree_log = id_oak_log;
		}

		// Find the highest layer.

		for (int y = 0; y < out->y_res; y++)
		{
			if (out->get_id(x, y, z) != id_air)
			{
				// Okay, found something that is not air.

				if (out->get_id(x, y, z) == id_grass)
				{
					// It's grass, we may plant a tree if there are enough 
					// blocks available above the grass.

					if (rand() % 2 == 0)
					{
						// Original tree.

						if (!out->in_bounds(x, y - 6, z))
						{
							goto tree;
						} 

						// This is the trunk.

						out->set_id_safe_if_air(x, y - 1, z, tree_log);
						out->set_id_safe_if_air(x, y - 2, z, tree_log);
						out->set_id_safe_if_air(x, y - 3, z, tree_log);
						out->set_id_safe_if_air(x, y - 4, z, tree_log);
						out->set_id_safe_if_air(x, y - 5, z, tree_log);

						// This is the cross at the top.

						out->set_id_safe_if_air(x, y - 6, z, tree_leaf);

						out->set_id_safe_if_air(x - 1, y - 6, z, tree_leaf);
						out->set_id_safe_if_air(x + 1, y - 6, z, tree_leaf);

						out->set_id_safe_if_air(x, y - 6, z - 1, tree_leaf);
						out->set_id_safe_if_air(x, y - 6, z + 1, tree_leaf);

						// This is the square at the layer second from the 
						// top.

						for (int j = -1; j <= 1; j++)
						{
							for (int k = -1; k <= 1; k++)
							{
								if (j == 0 && k == 0)
								{
									continue;
								}
								
								out->set_id_safe_if_air(x + j, y - 5, z + k, tree_leaf);
							}
						}

						// These are the squares at the third and fourth 
						// layer from the top.

						for (int j = -2; j <= 2; j++)
						{
							for (int k = -2; k <= 2; k++)
							{
								if (j == 0 && k == 0)
								{
									continue;
								}

								out->set_id_safe_if_air(x + j, y - 4, z + k, tree_leaf);
								out->set_id_safe_if_air(x + j, y - 3, z + k, tree_leaf);
							}
						}
					}
					else
					{
						// Rounded tree.

						if (!out->in_bounds(x, y - 7, z))
						{
							goto tree;
						} 

						// This is the trunk.

						out->set_id_safe_if_air(x, y - 1, z, tree_log);
						out->set_id_safe_if_air(x, y - 2, z, tree_log);
						out->set_id_safe_if_air(x, y - 3, z, tree_log);
						out->set_id_safe_if_air(x, y - 4, z, tree_log);
						out->set_id_safe_if_air(x, y - 5, z, tree_log);
						out->set_id_safe_if_air(x, y - 6, z, tree_log);

						// This is the cross at the top.

						out->set_id_safe_if_air(x, y - 7, z, tree_leaf);

						out->set_id_safe_if_air(x - 1, y - 7, z, tree_leaf);
						out->set_id_safe_if_air(x + 1, y - 7, z, tree_leaf);

						out->set_id_safe_if_air(x, y - 7, z - 1, tree_leaf);
						out->set_id_safe_if_air(x, y - 7, z + 1, tree_leaf);

						// This is the square at the layer second from the 
						// top.

						for (int j = -1; j <= 1; j++)
						{
							for (int k = -1; k <= 1; k++)
							{
								if (j == 0 && k == 0)
								{
									continue;
								}
								
								out->set_id_safe_if_air(x + j, y - 6, z + k, tree_leaf);
							}
						}

						// These are the squares at the third and fourth 
						// layer from the top.

						for (int j = -2; j <= 2; j++)
						{
							for (int k = -2; k <= 2; k++)
							{
								if (j == 0 && k == 0)
								{
									continue;
								}

								out->set_id_safe_if_air(x + j, y - 5, z + k, tree_leaf);
								out->set_id_safe_if_air(x + j, y - 4, z + k, tree_leaf);
							}
						}

						// This is the square at the fifth layer from the top.
						// It has it's corners removed.

						for (int j = -2; j <= 2; j++)
						{
							for (int k = -2; k <= 2; k++)
							{
								if (j == 0 && k == 0)
								{
									continue;
								}
								else if ((j == 2 && k == 2) || (j == 2 && k == -2) || (j == -2 && k == 2) || (j == -2 && k == -2))
								{
									continue;
								}
								
								out->set_id_safe_if_air(x + j, y - 3, z + k, tree_leaf);
							}
						}
					}
				}

				break;
			}
		}
	}

	done_trees:

	// Plant large mushrooms.

	int mushroom_count = out->x_res * out->z_res / 1024;

	if (mushroom_world)
	{
		mushroom_count = out->z_res * out->z_res / 32;
	}

	for (int i = 0; i < mushroom_count; i++)
	{
		mushroom:

		int x = rand() % out->x_res;
		int z = rand() % out->z_res;

		for (int y = 0; y < out->y_res; y++)
		{
			if (out->get_id(x, y, z) != id_air)
			{
				// Okay, found something that is not air.

				if (out->get_id(x, y, z) == id_grass || out->get_id(x, y, z) == id_mycelium)
				{
					// It's grass, we may plant a mushroom if there are enough
					// blocks available above the grass.

					if (!out->in_bounds(x, y - 6, z))
					{
						goto mushroom;
					}

					if (rand() % 5 != 0)
					{
						// Red mushroom. This is the trunk.

						out->set_id_safe_if_air(x, y - 1, z, id_mushroom_stem);
						out->set_id_safe_if_air(x, y - 2, z, id_mushroom_stem);
						out->set_id_safe_if_air(x, y - 3, z, id_mushroom_stem);
						out->set_id_safe_if_air(x, y - 4, z, id_mushroom_stem);
						out->set_id_safe_if_air(x, y - 5, z, id_mushroom_stem);

						// These are the five squares.

						for (int j = -1; j <= 1; j++)
						{
							for (int k = -1; k <= 1; k++)
							{
								// Top square.

								out->set_id_safe_if_air(x + j, y - 6, z + k, id_red_mushroom_block);

								// Left square.

								out->set_id_safe_if_air(x - 2, y - 4 + j, z + k, id_red_mushroom_block);

								out->set_id_safe_if_air(x - 1, y - 4 + j, z + k, id_air);

								// Right square.

								out->set_id_safe_if_air(x + 2, y - 4 + j, z + k, id_red_mushroom_block);

								out->set_id_safe_if_air(x + 1, y - 4 + j, z + k, id_air);

								// Front square.

								out->set_id_safe_if_air(x + k, y - 4 + j, z - 2, id_red_mushroom_block);

								out->set_id_safe_if_air(x + k, y - 4 + j, z - 1, id_air);

								// Back square.

								out->set_id_safe_if_air(x + k, y - 4 + j, z + 2, id_red_mushroom_block);

								out->set_id_safe_if_air(x + k, y - 4 + j, z + 1, id_air);
							}
						}
					}
					else
					{
						// Brown mushroom. This is the trunk.

						out->set_id_safe_if_air(x, y - 1, z, id_mushroom_stem);
						out->set_id_safe_if_air(x, y - 2, z, id_mushroom_stem);
						out->set_id_safe_if_air(x, y - 3, z, id_mushroom_stem);
						out->set_id_safe_if_air(x, y - 4, z, id_mushroom_stem);
						out->set_id_safe_if_air(x, y - 5, z, id_mushroom_stem);

						for (int j = -3; j <= 3; j++)
						{
							for (int k = -3; k <= 3; k++)
							{
								if ((j == 3 && k == 3) || (j == 3 && k == -3) || (j == -3 && k == 3) || (j == -3 && k == -3))
								{
									continue;
								}

								out->set_id_safe_if_air(x + j, y - 6, z + k, id_brown_mushroom_block);
							}
						}
					}
				}
			}
		}
	}

	// For each vertical strip of land, start with a lighting value of 15. Go
	// downwards, if the block is air, stay at light level 15. If not, set 
	// everything below it to 0.

	for (int x = 0; x < out->x_res; x++)
	{
		for (float z = 0; z < out->z_res; z++)
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