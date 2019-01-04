// Choose a growth timer for a plant that has 8 stages of growth.

inline unsigned int growth_8()
{
	return 120 + (rand() % 120) - (rand() % 120); 
}

// Choose a growth timer for a plant that has 4 stages of growth.

inline unsigned int growth_4()
{
	return growth_8() * 2; 
}

// Choose a growth timer for a growing tree/sapling.

inline unsigned int growth_tree()
{
	return rand() % 60;
}

// Types of growing plants.

enum growing_plant_type
{
	growing_wheat,

	growing_carrots,

	growing_potatoes,

	growing_beetroots,

	growing_oak_sapling,

	growing_birch_sapling
};

// A growing plant.

struct growing_plant
{
	// The position of this growing_plant.

	unsigned int x;
	unsigned int y;
	unsigned int z;

	// The block that this growing_plant should be. If it is not, that means
	// that something else modified it and it should not grow anymore.

	block_id should_id;

	// The growing_plant_type of this growing_plant.

	growing_plant_type type;

	// The amount of ticks until the plant grows into it's next form. This
	// value is decremented by 1 every tick. If it is equal to 0, the plant 
	// grows into it's next form (if any). The timer is then set to the amount
	// of ticks until the plant grows into it's following form.

	unsigned int timer;

	// A growing plant is marked as done when it has reached it's final form
	// or phase.

	bool done;

	growing_plant
	(
		unsigned int _x,
		unsigned int _y,
		unsigned int _z,

		block_id _should_id,

		growing_plant_type _type,

		unsigned int _timer
	)
	{
		x = _x;
		y = _y;
		z = _z;

		should_id = _should_id;

		type = _type;

		timer = _timer;

		done = false;
	}
};