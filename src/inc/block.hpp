#include <vector>
#include <string>

// An enumeration of all the implemented blocks.

enum block_id
{
	id_air,

	id_bedrock,

	id_water,

	id_stone,

	id_cobblestone,

	id_dirt,

	id_grass,

	id_mycelium,

	id_sand,

	id_gravel,

	id_brick,

	id_bookshelf,

	id_oak_log,

	id_oak_planks,

	id_oak_leaves,

	id_birch_log,

	id_birch_planks,

	id_birch_leaves,

	id_red_mushroom_block,

	id_brown_mushroom_block,

	id_mushroom_stem,

	id_coal_ore,

	id_iron_ore,

	id_gold_ore,

	id_diamond_ore,

	id_emerald_ore,

	id_white_wool,

	id_orange_wool,

	id_magenta_wool,

	id_light_blue_wool,

	id_yellow_wool,

	id_lime_wool,

	id_pink_wool,

	id_gray_wool,

	id_light_gray_wool,

	id_cyan_wool,

	id_purple_wool,

	id_blue_wool,

	id_brown_wool,

	id_green_wool,

	id_red_wool,

	id_black_wool,

	id_last
};

// Conversion from block_id to std::string.

std::vector<std::string> block_id_to_string =
{
	"air",

	"bedrock",

	"water",

	"stone",

	"cobblestone",

	"dirt",

	"grass",

	"mycelium",

	"sand",

	"gravel",

	"brick",

	"bookshelf",

	"oak_log",

	"oak_planks",

	"oak_leaves",

	"birch_log",

	"birch_planks",

	"birch_leaves",

	"red_mushroom_block",

	"brown_mushroom_block",

	"mushroom_stem",

	"coal_ore",

	"iron_ore",

	"gold_ore",

	"diamond_ore",

	"emerald_ore",

	"white_wool",

	"orange_wool",

	"magenta_wool",

	"light_blue_wool",

	"yellow_wool",

	"lime_wool",

	"pink_wool",

	"gray_wool",

	"light_gray_wool",

	"cyan_wool",

	"purple_wool",

	"blue_wool",

	"brown_wool",

	"green_wool",

	"red_wool",

	"black_wool"
};

// Conversion from block_id to std::string (used for in-game text).

std::vector<std::string> block_id_to_block_name =
{
	"Air",

	"Bedrock",

	"Water",

	"Stone",

	"Cobblestone",

	"Dirt",

	"Grass",

	"Mycelium",

	"Sand",

	"Gravel",

	"Brick",

	"Bookshelf",

	"Oak Log",

	"Oak Planks",

	"Oak Leaves",

	"Birch Log",

	"Birch Planks",

	"Birch Leaves",

	"Red Mushroom Block",

	"Brown Mushroom Block",

	"Mushroom Stem",

	"Coal Ore",

	"Iron Ore",

	"Gold Ore",

	"Diamond Ore",

	"Emerald Ore",

	"White Wool",

	"Orange Wool",

	"Magenta Wool",

	"Light Blue Wool",

	"Yellow Wool",

	"Lime Wool",

	"Pink Wool",

	"Gray Wool",

	"Light Gray Wool",

	"Cyan Wool",

	"Purple Wool",

	"Blue Wool",

	"Brown Wool",

	"Green Wool",

	"Red Wool",

	"Black Wool"
};