#include <string>
#include <fstream>

// Save a world to a file.

void save_world_to_file(world* in, float player_x, float player_y, float player_z, std::string path)
{
	std::ofstream out(path, std::ios::binary);

	// Write the world's size to the output stream.

	unsigned int size_data[3];

	size_data[0] = in->x_res;
	size_data[1] = in->y_res;
	size_data[2] = in->z_res;

	out.write((char*)size_data, sizeof(size_data));

	// Write the player's coordinates to the output stream.

	float player_coordinates[3];

	player_coordinates[0] = player_x;
	player_coordinates[1] = player_y;
	player_coordinates[2] = player_z;

	out.write((char*)player_coordinates, sizeof(player_coordinates));

	// Write the world's voxel data to the output stream.

	out.write((char*)in->voxels, in->x_res * in->y_res * in->z_res * sizeof(voxel));

	// Close the output stream.

	out.close();
}

// Load a world from a file.

void load_world_from_file(world*& out, float& player_x, float& player_y, float& player_z, std::string path)
{
	std::ifstream in(path, std::ios::binary);

	// Read the world's size from the input stream.

	unsigned int size_data[3];

	in.read((char*)size_data, sizeof(size_data));

	// Read the player's coordinates from the output stream.

	float player_coordinates[3];

	in.read((char*)player_coordinates, sizeof(player_coordinates));

	player_x = player_coordinates[0];
	player_y = player_coordinates[1];
	player_z = player_coordinates[2];

	// Read the world's voxel data from the input stream after allocating a
	// new world.

	out = allocate_world(size_data[0], size_data[1], size_data[2]);

	in.read((char*)out->voxels, out->x_res * out->y_res * out->z_res * sizeof(voxel));

	// Close the input stream.

	in.close();
}