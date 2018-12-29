// A world struct hold information about a worlds dimensions and block data. 
// It makes it easy and efficient to modify and read the properties of every
// voxel it contains.

struct world
{
	voxel* voxels;

	unsigned int x_res;
	unsigned int y_res;
	unsigned int z_res;
};