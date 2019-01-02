#include <vector>

// A struct of type face_info represents the layer (w coordinate) of each face
// of a block. It is used to map the correct texture onto each face of a 
// block.

struct face_info
{
	float l_top;

	float l_bottom;

	float l_left;

	float l_right;

	float l_front;

	float l_back;

	face_info(float _l_top, float _l_bottom, float _l_left, float _l_right, float _l_front, float _l_back)
	{
		l_top = _l_top;

		l_bottom = _l_bottom;

		l_left = _l_left;

		l_right = _l_right;

		l_front = _l_front;

		l_back = _l_back;
	}
};

// Generates a face_info* where each all of the six faces correspond to the
// same layer.

face_info* make_face_info_all(float all)
{
	return new face_info(all, all, all, all, all, all);
}

// Generates a face_info* that uses cap for the top and bottom faces, and uses
// side for the remaining four faces.

face_info* make_face_info_cap(float cap, float side)
{
	return new face_info(cap, cap, side, side, side, side);
}

// Generates a face_info* that uses top for the top face, uses bottom for the
// bottom face, and uses side for the remaining four faces.

face_info* make_face_info_caps(float top, float bottom, float side)
{
	return new face_info(top, bottom, side, side, side, side);
}

// This function fills up block_face_info with a face_info* object for each
// block_id. Call this function after calling load_block_texture_array, 
// because this function uses block_name_to_layer as a lookup table.

std::vector<face_info*> block_face_info;

void load_block_face_info_array()
{
	block_face_info = 
	{
		make_face_info_all(-1.0f),

		make_face_info_all(block_name_to_layer.at("bedrock")),

		make_face_info_all(block_name_to_layer.at("water_0")),

		make_face_info_all(block_name_to_layer.at("stone")),

		make_face_info_all(block_name_to_layer.at("cobblestone")),

		make_face_info_all(block_name_to_layer.at("dirt")),

		make_face_info_caps(block_name_to_layer.at("grass_top"), block_name_to_layer.at("dirt"), block_name_to_layer.at("grass_side")),

		make_face_info_caps(block_name_to_layer.at("mycelium_top"), block_name_to_layer.at("dirt"), block_name_to_layer.at("mycelium_side")),

		make_face_info_all(block_name_to_layer.at("sand")),

		make_face_info_caps(block_name_to_layer.at("sandstone_top"), block_name_to_layer.at("sandstone_bottom"), block_name_to_layer.at("sandstone_side")),

		make_face_info_all(block_name_to_layer.at("gravel")),

		make_face_info_all(block_name_to_layer.at("brick")),

		make_face_info_cap(block_name_to_layer.at("oak_planks"), block_name_to_layer.at("bookshelf")),

		make_face_info_cap(block_name_to_layer.at("oak_log_top"), block_name_to_layer.at("oak_log_side")),

		make_face_info_all(block_name_to_layer.at("oak_planks")),

		make_face_info_all(block_name_to_layer.at("oak_leaves")),

		make_face_info_cap(block_name_to_layer.at("birch_log_top"), block_name_to_layer.at("birch_log_side")),

		make_face_info_all(block_name_to_layer.at("birch_planks")),

		make_face_info_all(block_name_to_layer.at("birch_leaves")),

		make_face_info_all(block_name_to_layer.at("mushroom_block_red")),

		make_face_info_all(block_name_to_layer.at("mushroom_block_brown")),

		make_face_info_cap(block_name_to_layer.at("mushroom_block_stem_top"), block_name_to_layer.at("mushroom_block_stem_side")),

		make_face_info_all(block_name_to_layer.at("coal_ore")),

		make_face_info_all(block_name_to_layer.at("iron_ore")),

		make_face_info_all(block_name_to_layer.at("gold_ore")),

		make_face_info_all(block_name_to_layer.at("diamond_ore")),

		make_face_info_all(block_name_to_layer.at("emerald_ore")),

		make_face_info_all(block_name_to_layer.at("wool_colored_white")),

		make_face_info_all(block_name_to_layer.at("wool_colored_orange")),

		make_face_info_all(block_name_to_layer.at("wool_colored_magenta")),

		make_face_info_all(block_name_to_layer.at("wool_colored_light_blue")),

		make_face_info_all(block_name_to_layer.at("wool_colored_yellow")),

		make_face_info_all(block_name_to_layer.at("wool_colored_lime")),

		make_face_info_all(block_name_to_layer.at("wool_colored_pink")),

		make_face_info_all(block_name_to_layer.at("wool_colored_gray")),

		make_face_info_all(block_name_to_layer.at("wool_colored_silver")),

		make_face_info_all(block_name_to_layer.at("wool_colored_cyan")),

		make_face_info_all(block_name_to_layer.at("wool_colored_purple")),

		make_face_info_all(block_name_to_layer.at("wool_colored_blue")),

		make_face_info_all(block_name_to_layer.at("wool_colored_brown")),

		make_face_info_all(block_name_to_layer.at("wool_colored_green")),

		make_face_info_all(block_name_to_layer.at("wool_colored_red")),

		make_face_info_all(block_name_to_layer.at("wool_colored_black")),

		make_face_info_all(block_name_to_layer.at("glass")),

		make_face_info_all(block_name_to_layer.at("rose")),

		make_face_info_all(block_name_to_layer.at("dandelion")),

		make_face_info_all(block_name_to_layer.at("red_mushroom")),

		make_face_info_all(block_name_to_layer.at("brown_mushroom")),

		make_face_info_cap(block_name_to_layer.at("stone_slab_top"), block_name_to_layer.at("stone_slab_side")),

		make_face_info_caps(block_name_to_layer.at("sandstone_top"), block_name_to_layer.at("sandstone_bottom"), block_name_to_layer.at("sandstone_side")),

		make_face_info_all(block_name_to_layer.at("oak_planks")),

		make_face_info_all(block_name_to_layer.at("birch_planks")),

		make_face_info_all(block_name_to_layer.at("cobblestone")),

		make_face_info_all(block_name_to_layer.at("brick")),

		make_face_info_cap(block_name_to_layer.at("stone_slab_top"), block_name_to_layer.at("stone_slab_side")),
		make_face_info_all(block_name_to_layer.at("brick"))
	};
}