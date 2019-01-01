// Returns true if a face should be shown, otherwise returns false.

inline bool show_face(block_id face, block_id neighbor, int face_id)
{
	bool hide = is_transparent(neighbor);

	if (face == id_water && neighbor == id_water)
	{
		hide = false;
	}
	else if (face == id_glass && neighbor == id_glass)
	{
		hide = false;
	}

	return hide;
}

// Convert a subset of a world into a vertex array. The generated vertex 
// array is stored in target, and it's size in floats is stored in 
// target_size_in_floats.

void world_subset_to_mesh
(
	world* input,

	unsigned int x,
	unsigned int y,
	unsigned int z,

	unsigned int x_res,
	unsigned int y_res,
	unsigned int z_res,

	float* target,

	unsigned int& target_size_in_floats
)
{
	float* ptr = target;

	unsigned int fx = x + x_res;
	unsigned int fy = y + y_res;
	unsigned int fz = z + z_res;

	for (unsigned int cx = x; cx < fx; cx++)
	{
		for (unsigned int cy = y; cy < fy; cy++)
		{
			for (unsigned int cz = z; cz < fz; cz++)
			{
				// Skip voxels that are out of bounds.

				if (!input->in_bounds(cx, cy, cz))
				{
					continue;
				}

				// Get the current voxel's block_id.

				block_id voxel_id = input->get_id(cx, cy, cz);

				// Ignore voxels that have a block_id equivalent to id_air or 
				// id_water.

				if (voxel_id == id_air || voxel_id == id_water)
				{
					continue;
				}

				// Get the face_info* object that matches the block_id of the
				// current voxel.

				face_info* cube_face_info = block_face_info[voxel_id];

				// Get the layer (w coordinate) corresponding to each face of 
				// the current voxel.

				float layer_top = cube_face_info->l_top;

				float layer_bottom = cube_face_info->l_bottom;

				float layer_left = cube_face_info->l_left;

				float layer_right = cube_face_info->l_right;

				float layer_front = cube_face_info->l_front;

				float layer_back = cube_face_info->l_back;

				// Calculate the lighting value of each face by multiplying 
				// the final lighting value of the neighboring voxels by a 
				// constant coefficient.

				float lighting_top = 1.0f * std::max(input->get_natural_edge(cx, cy, cz, cx, cy - 1, cz) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx, cy - 1, cz) / 15.0f);

				float lighting_bottom = 0.65f * std::max(input->get_natural_edge(cx, cy, cz, cx, cy + 1, cz) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx, cy + 1, cz) / 15.0f);

				float lighting_left = 0.75f * std::max(input->get_natural_edge(cx, cy, cz, cx - 1, cy, cz) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx - 1, cy, cz) / 15.0f);

				float lighting_right = 0.75f * std::max(input->get_natural_edge(cx, cy, cz, cx + 1, cy, cz) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx + 1, cy, cz) / 15.0f);

				float lighting_front = 0.9f * std::max(input->get_natural_edge(cx, cy, cz, cx, cy, cz - 1) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx, cy, cz - 1) / 15.0f);

				float lighting_back = 0.9f * std::max(input->get_natural_edge(cx, cy, cz, cx, cy, cz + 1) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx, cy, cz + 1) / 15.0f);

				// Do hidden face culling. This optimization will cause faces
				// that are never going to be rendered (hidden faces) to be 
				// culled.

				bool visible_top = show_face(voxel_id, input->get_id_safe(cx, cy - 1, cz), 0);

				bool visible_bottom = show_face(voxel_id, input->get_id_safe(cx, cy + 1, cz), 1);

				bool visible_left = show_face(voxel_id, input->get_id_safe(cx - 1, cy, cz), 2);

				bool visible_right = show_face(voxel_id, input->get_id_safe(cx + 1, cy, cz), 3);

				bool visible_front = show_face(voxel_id, input->get_id_safe(cx, cy, cz - 1), 4);

				bool visible_back = show_face(voxel_id, input->get_id_safe(cx, cy, cz + 1), 5);

				// Generate visible faces and write them to the target array,
				// using ptr as a 'stream writer'.

				if (visible_top)
				{
					*(ptr++) = 1.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
					*(ptr++) = 0.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
					*(ptr++) = 0.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;

					*(ptr++) = 1.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
					*(ptr++) = 0.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
					*(ptr++) = 1.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
				}

				if (visible_bottom)
				{
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;

					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
				}

				if (visible_left)
				{
					*(ptr++) = 0.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;

					*(ptr++) = 0.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
					*(ptr++) = 0.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
				}

				if (visible_right)
				{
					*(ptr++) = 1.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;

					*(ptr++) = 1.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
					*(ptr++) = 1.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
				}

				if (visible_front)
				{
					*(ptr++) = 0.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;

					*(ptr++) = 0.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
					*(ptr++) = 1.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
				}

				if (visible_back)
				{
					*(ptr++) = 1.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;

					*(ptr++) = 1.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
					*(ptr++) = 0.0f + cx; *(ptr++) = -0.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
				}
			}
		}
	}

	// Calculate the amount of floats that were written to target, and store
	// that value in target_size_in_floats.

	target_size_in_floats = ptr - target;
}

// Convert a subset of a world into a water vertex array. The generated water
// vertex array is stored in water_target, and it's size in floats is stored 
// in water_target_size_in_floats.

void world_subset_to_water_mesh
(
	world* input,

	unsigned int x,
	unsigned int y,
	unsigned int z,

	unsigned int x_res,
	unsigned int y_res,
	unsigned int z_res,

	float* water_target,

	unsigned int& water_target_size_in_floats
)
{
	float* ptr = water_target;

	unsigned int fx = x + x_res;
	unsigned int fy = y + y_res;
	unsigned int fz = z + z_res;

	for (unsigned int cx = x; cx < fx; cx++)
	{
		for (unsigned int cy = y; cy < fy; cy++)
		{
			for (unsigned int cz = z; cz < fz; cz++)
			{
				// Skip voxels that are out of bounds.

				if (!input->in_bounds(cx, cy, cz))
				{
					continue;
				}

				// Get the current voxel's block_id.

				block_id voxel_id = input->get_id(cx, cy, cz);

				// Ignore voxels that have a block_id that is not equivalent
				// to water.

				if (voxel_id != id_water)
				{
					continue;
				}

				// Get the face_info* object that matches the block_id of the
				// current voxel.

				face_info* cube_face_info = block_face_info[voxel_id];

				// Get the layer (w coordinate) corresponding to each face of 
				// the current voxel.

				float layer_top = -cube_face_info->l_top;

				float layer_bottom = -cube_face_info->l_bottom;

				float layer_left = -cube_face_info->l_left;

				float layer_right = -cube_face_info->l_right;

				float layer_front = -cube_face_info->l_front;

				float layer_back = -cube_face_info->l_back;

				// Calculate the lighting value of each face by multiplying 
				// the final lighting value of the neighboring voxels by a 
				// constant coefficient.

				float lighting_top = 1.0f * std::max(input->get_natural_edge(cx, cy, cz, cx, cy - 1, cz) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx, cy - 1, cz) / 15.0f);

				float lighting_bottom = 0.65f * std::max(input->get_natural_edge(cx, cy, cz, cx, cy + 1, cz) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx, cy + 1, cz) / 15.0f);

				float lighting_left = 0.75f * std::max(input->get_natural_edge(cx, cy, cz, cx - 1, cy, cz) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx - 1, cy, cz) / 15.0f);

				float lighting_right = 0.75f * std::max(input->get_natural_edge(cx, cy, cz, cx + 1, cy, cz) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx + 1, cy, cz) / 15.0f);

				float lighting_front = 0.9f * std::max(input->get_natural_edge(cx, cy, cz, cx, cy, cz - 1) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx, cy, cz - 1) / 15.0f);

				float lighting_back = 0.9f * std::max(input->get_natural_edge(cx, cy, cz, cx, cy, cz + 1) / 15.0f, input->get_artificial_edge(cx, cy, cz, cx, cy, cz + 1) / 15.0f);

				// Do hidden face culling. This optimization will cause faces
				// that are never going to be rendered (hidden faces) to be 
				// culled.

				bool visible_top = show_face(voxel_id, input->get_id_safe(cx, cy - 1, cz), 0);

				bool visible_bottom = show_face(voxel_id, input->get_id_safe(cx, cy + 1, cz), 1);

				bool visible_left = show_face(voxel_id, input->get_id_safe(cx - 1, cy, cz), 2);

				bool visible_right = show_face(voxel_id, input->get_id_safe(cx + 1, cy, cz), 3);

				bool visible_front = show_face(voxel_id, input->get_id_safe(cx, cy, cz - 1), 4);

				bool visible_back = show_face(voxel_id, input->get_id_safe(cx, cy, cz + 1), 5);

				// Top level water should be rendered as a short block (15/16 pixels tall).

				float top_level_water = 0.0f;

				// Generate visible faces and write them to the target array,
				// using ptr as a 'stream writer'.

				if (visible_top)
				{
					*(ptr++) = 1.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
					*(ptr++) = 0.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
					*(ptr++) = 0.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;

					*(ptr++) = 1.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
					*(ptr++) = 0.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
					*(ptr++) = 1.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
				}

				if (visible_bottom)
				{
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;

					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
				}

				if (visible_left)
				{
					*(ptr++) = 0.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;

					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;

					*(ptr++) = 0.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;

					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;

					*(ptr++) = 0.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
				}

				if (visible_right)
				{
					*(ptr++) = 1.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;

					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;

					*(ptr++) = 1.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;

					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;

					*(ptr++) = 1.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
				}

				if (visible_front)
				{
					*(ptr++) = 0.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;

					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;

					*(ptr++) = 0.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;

					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;

					*(ptr++) = 1.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 0.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
				}

				if (visible_back)
				{
					*(ptr++) = 1.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;

					*(ptr++) = 1.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;

					*(ptr++) = 1.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;

					*(ptr++) = 0.0f + cx; *(ptr++) = -1.0f - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;

					*(ptr++) = 0.0f + cx; *(ptr++) = -top_level_water - cy; *(ptr++) = 1.0f + cz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
				}
			}
		}
	}

	// Calculate the amount of floats that were written to water_target, and 
	// store that value in water_target_size_in_floats.

	water_target_size_in_floats = ptr - water_target;
}