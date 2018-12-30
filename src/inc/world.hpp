#include <iostream>

// A world struct holds information about a world's dimensions and block data.
// It makes it easy and efficient to modify and read the properties of every
// voxel it contains.

struct world
{
	voxel* voxels;

	unsigned int x_res;
	unsigned int y_res;
	unsigned int z_res;

	// Get the voxel at the specified coordinates.

	inline voxel get(unsigned int x, unsigned int y, unsigned int z)
	{
		return voxels[x + x_res * (y + y_res * z)];
	}

	// Get the voxel at the specified coordinates, if the coordinates are 
	// within the bounds of the world. If not, return 0.

	inline voxel get_safe(unsigned int x, unsigned int y, unsigned int z)
	{
		if (x < 0 || y < 0 || z < 0 || x > x_res - 1 || y > y_res - 1 || z > z_res - 1)
		{
			return 0;
		}

		return voxels[x + x_res * (y + y_res * z)];
	}

	// Get the block_id information of the voxel at the specified coordinates.

	inline block_id get_id(unsigned int x, unsigned int y, unsigned int z)
	{
		return voxel_get_id(voxels[x + x_res * (y + y_res * z)]);
	}

	// Get the block_id information of the voxel at the specified coordinates,
	// if the coordinates are within the bounds of the world. If not, return 
	// id_null.

	inline block_id get_id_safe(unsigned int x, unsigned int y, unsigned int z)
	{
		if (x < 0 || y < 0 || z < 0 || x > x_res - 1 || y > y_res - 1 || z > z_res - 1)
		{
			return id_null;
		}

		return voxel_get_id(voxels[x + x_res * (y + y_res * z)]);
	}

	// Get the natural lighting information of the voxel at the specified 
	// coordinates.

	inline unsigned char get_natural(unsigned int x, unsigned int y, unsigned int z)
	{
		return voxel_get_natural(voxels[x + x_res * (y + y_res * z)]);
	}

	// Get the natural lighting information of the voxel at the specified 
	// coordinates, if the coordinates are within the bounds of the world. If
	// not, return 0.

	inline unsigned char get_natural_safe(unsigned int x, unsigned int y, unsigned int z)
	{
		if (x < 0 || y < 0 || z < 0 || x > x_res - 1 || y > y_res - 1 || z > z_res - 1)
		{
			return 0;
		}

		return voxel_get_natural(voxels[x + x_res * (y + y_res * z)]);
	}

	// Get the artificial lighting information of the voxel at the specified 
	// coordinates.

	inline unsigned char get_artificial(unsigned int x, unsigned int y, unsigned int z)
	{
		return voxel_get_artificial(voxels[x + x_res * (y + y_res * z)]);
	}

	// Get the artificial lighting information of the voxel at the specified 
	// coordinates, if the coordinates are within the bounds of the world. If
	// not, return 0.

	inline unsigned char get_artificial_safe(unsigned int x, unsigned int y, unsigned int z)
	{
		if (x < 0 || y < 0 || z < 0 || x > x_res - 1 || y > y_res - 1 || z > z_res - 1)
		{
			return 0;
		}

		return voxel_get_artificial(voxels[x + x_res * (y + y_res * z)]);
	}

	// Set the block_id information of the voxel at the specified coordinates.

	inline void set_id(unsigned int x, unsigned int y, unsigned int z, block_id id)
	{
		voxel_set_id(voxels[x + x_res * (y + y_res * z)], id);
	}

	// Set the block_id information of the voxel at the specified coordinates,
	// if the current block_id of the voxel at the specified coordinates is 
	// id_air.

	inline void set_id_if_air(unsigned int x, unsigned int y, unsigned int z, block_id id)
	{
		if (get_id(x, y, z) == id_air)
		{
			voxel_set_id(voxels[x + x_res * (y + y_res * z)], id);
		}
	}

	// Set the block_id information of the voxel at the specified coordinates,
	// if the current block_id of the voxel at the specified coordinates is 
	// not id_air.

	inline void set_id_if_not_air(unsigned int x, unsigned int y, unsigned int z, block_id id)
	{
		if (get_id(x, y, z) != id_air)
		{
			voxel_set_id(voxels[x + x_res * (y + y_res * z)], id);
		}
	}

	// Set the block_id information of the voxel at the specified coordinates,
	// if the coordinates are within the bounds of the world.

	inline void set_id_safe(unsigned int x, unsigned int y, unsigned int z, block_id id)
	{
		if (x < 0 || y < 0 || z < 0 || x > x_res - 1 || y > y_res - 1 || z > z_res - 1)
		{
			return;
		}

		voxel_set_id(voxels[x + x_res * (y + y_res * z)], id);
	}

	// Set the block_id information of the voxel at the specified coordinates,
	// if the coordinates are within the bounds of the world, and if the 
	// current block_id of the voxel at the specified coordinates is id_air.

	inline void set_id_safe_if_air(unsigned int x, unsigned int y, unsigned int z, block_id id)
	{
		if (x < 0 || y < 0 || z < 0 || x > x_res - 1 || y > y_res - 1 || z > z_res - 1)
		{
			return;
		}

		if (get_id(x, y, z) == id_air)
		{
			voxel_set_id(voxels[x + x_res * (y + y_res * z)], id);
		}
	}

	// Set the natural lighting information of the voxel at the specified 
	// coordinates.

	inline void set_natural(unsigned int x, unsigned int y, unsigned int z, unsigned char natural)
	{
		voxel_set_natural(voxels[x + x_res * (y + y_res * z)], natural);
	}

	// Set the natural lighting information of the voxel at the specified 
	// coordinates, if the coordinates are within the bounds of the world.

	inline void set_natural_safe(unsigned int x, unsigned int y, unsigned int z, unsigned char natural)
	{
		if (x < 0 || y < 0 || z < 0 || x > x_res - 1 || y > y_res - 1 || z > z_res - 1)
		{
			return;
		}

		voxel_set_natural(voxels[x + x_res * (y + y_res * z)], natural);
	}

	// Set the artificial lighting information of the voxel at the specified 
	// coordinates.

	inline void set_artificial(unsigned int x, unsigned int y, unsigned int z, unsigned char artificial)
	{
		voxel_set_artificial(voxels[x + x_res * (y + y_res * z)], artificial);
	}

	// Set the artificial lighting information of the voxel at the specified 
	// coordinates, if the coordinates are within the bounds of the world.

	inline void set_artificial_safe(unsigned int x, unsigned int y, unsigned int z, unsigned char artificial)
	{
		if (x < 0 || y < 0 || z < 0 || x > x_res - 1 || y > y_res - 1 || z > z_res - 1)
		{
			return;
		}

		voxel_set_artificial(voxels[x + x_res * (y + y_res * z)], artificial);
	}

	// Check if this world may reference a voxel at the given coordinates. If
	// so, return true. Else, return false.

	inline bool in_bounds(unsigned int x, unsigned int y, unsigned int z)
	{
		if (x < 0 || y < 0 || z < 0 || x > x_res - 1 || y > y_res - 1 || z > z_res - 1)
		{
			return false;
		}

		return true;
	}
};

// Allocate an empty world* of a given resolution. Each voxel in the world 
// will be set to 0.

world* allocate_world(unsigned int x_res, unsigned int y_res, unsigned int z_res)
{
	world* new_world = new world();

	new_world->voxels = (voxel*)malloc(x_res * y_res * z_res * sizeof(voxel));

	if (!new_world->voxels)
	{
		std::cout << "Could not allocate enough memory for a new world." << std::endl;

		exit(13);
	}

	memset(new_world->voxels, 0, x_res * y_res * z_res * sizeof(voxel));

	new_world->x_res = x_res;
	new_world->y_res = y_res;
	new_world->z_res = z_res;

	return new_world;
}

// Deallocate a world*'s voxels, and then delete the world*.

void deallocate_world(world* to_be_annihilated)
{
	free(to_be_annihilated->voxels);

	delete to_be_annihilated;
}