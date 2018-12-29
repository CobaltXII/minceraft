// A world struct hold information about a worlds dimensions and block data. 
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
};