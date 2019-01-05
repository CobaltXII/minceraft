// Choose a timer for a burning_fire.

inline unsigned int fire_timer()
{
	return 300 + (rand() % 60) - (rand() % 60);
}

// A burning fire.

struct burning_fire
{
	// The position of this burning_fire.

	unsigned int x;
	unsigned int y;
	unsigned int z;

	// The block that this burning_fire should be. If it is not, that means
	// that something else modified it and it should not burn anymore.

	block_id should_id;

	// The amount of ticks until the fire disappears. While this timer is 
	// above 0, this fire has a chance to spread to any of it's flammable 
	// neighbors.

	unsigned int timer;

	// A burning_fire is marked as done when it has reached it's final form
	// or phase.

	bool done;

	burning_fire
	(
		unsigned int _x,
		unsigned int _y,
		unsigned int _z,

		unsigned int _timer
	)
	{
		x = _x;
		y = _y;
		z = _z;

		timer = _timer;

		done = false;
	}
};