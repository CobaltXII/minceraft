// An accessor struct acts as a middleman between common code and world 
// objects. It's job is to queue up all the changes made to a world, and
// then regenerate all required resources with a single function call.

struct accessor
{
	world* the_world;

	chunk** the_chunks;

	unsigned int chunk_x_res;
	unsigned int chunk_y_res;
	unsigned int chunk_z_res;

	unsigned int chunk_count;
};
