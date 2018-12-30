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
				out->set_id_safe(int(x), int(y), int(z), id_grass);

				out->set_id_safe_if_not_air(int(x), int(y) + 1, int(z), id_dirt);
				out->set_id_safe_if_not_air(int(x), int(y) + 2, int(z), id_dirt);
				out->set_id_safe_if_not_air(int(x), int(y) + 3, int(z), id_dirt);
			}

			break;
		}
	}

	// Add water.

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
			else if (out->get_id(int(x), int(y), int(z)) == id_grass)
			{
				out->set_id(int(x), int(y), int(z), id_sand);
			}
		}
	}

	// Plant trees.

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
					// It's grass, we may plant a tree if there are enough blocks available above 
					// the grass.

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

						// This is the square at the layer second from the top.

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

						// These are the squares at the third and fourth layer from the top.

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

						// This is the square at the layer second from the top.

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

						// These are the squares at the third and fourth layer from the top.

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

						// This is the square at the fifth layer from the top. It has it's corners
						// removed.

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

	// Do natural lighting calculations.

	for (unsigned int x = 0; x < out->x_res; x++)
	for (unsigned int y = 0; y < out->y_res; y++)
	for (unsigned int z = 0; z < out->z_res; z++)
	{
		out->set_natural(x, y, z, 15);
	}
}