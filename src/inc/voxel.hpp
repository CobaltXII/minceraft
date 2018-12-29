// A voxel can be represented with 16 bits of data. The last 8 bits hold the
// block_id information (that allows 256 distinct block_id values). The first 
// 4 bits hold the natural light information, and the second 4 bits hold the 
// artificial light information. That allows for 16 unique lighting values for
// each lighting component, and 256 unique lighting permutations in total.

typedef unsigned short voxel;

// Get the block_id information of a voxel. The block_id information of a 
// voxel is stored in the last 8 bits of the voxel.

inline block_id voxel_get_id(voxel which)
{
	return block_id(which & 0xff);
}

// Set the block_id information of a voxel. The block_id information of a 
// voxel is stored in the last 8 bits of the voxel.

inline voxel voxel_set_id(voxel& which, block_id id)
{
	return which = (which & 0xff00) | (id & 0xff);
}

// Get the natural light information of a voxel. The natural light information
// of a voxel is stored in the first 4 bits of the voxel.

inline unsigned char voxel_get_natural(voxel which)
{
	return which >> 12;
}

// Set the natural light information of a voxel. The natural light information
// of a voxel is stored in the first 4 bits of the voxel.

inline voxel voxel_set_natural(voxel& which, unsigned char natural)
{
	return which = (which & 0xfff) | ((natural & 0xf) << 12);
}

// Get the artificial light information of a voxel. The artificial light 
// information of a voxel is stored in the second 4 bits of the voxel.

inline unsigned char voxel_get_artificial(voxel which)
{
	return (which >> 8) & 0xf;
}

// Set the artificial light information of a voxel. The artificial light 
// information of a voxel is stored in the second 4 bits of the voxel.

inline unsigned char voxel_set_artificial(voxel& which, unsigned char artificial)
{
	return which = (which & 0xf0ff) | ((artificial & 0xf) << 8);
}