/*

Minceraft

By CobaltXII

*/

#include "main.hpp"

// The entry point.

int main(int argc, char** argv)
{
	// Take command line arguments.

	std::string path_to_level = "level.dat";

	if (argc == 2)
	{
		path_to_level = std::string(argv[1]);
	}
	else if (argc != 1)
	{
		std::cout << "Usage: " << argv[0] << " [path-to-level]" << std::endl;

		exit(16);
	}

	// Initialize SDL.

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		std::cout << "Could not initialize SDL." << std::endl;

		exit(1);
	}

	// The dimensions of the SDL_Window*.

	int sdl_x_res = 950;
	int sdl_y_res = 540;

	// Create the SDL_Window*.

	SDL_Window* sdl_window = SDL_CreateWindow
	(
		"Minceraft 0.2.45",

		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,

		sdl_x_res,
		sdl_y_res,

		SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL
	);

	// Make sure the SDL_Window* was created successfully.

	if (!sdl_window)
	{
		std::cout << "Could not create the SDL_Window*." << std::endl;

		exit(2);
	}

	// Request OpenGL 3.3 (core profile).

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Request double-buffering.

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Request a 24-bit depth buffer.

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Enable vertical retrace synchronization.

	SDL_GL_SetSwapInterval(1);

	// Create the SDL_GLContext.

	SDL_GLContext gl_context = SDL_GL_CreateContext(sdl_window);

	if (!gl_context)
	{
		std::cout << "Could not create the SDL_GLContext." << std::endl;

		exit(3);
	}

	// Load all OpenGL 3.3 (core profile) functions using GLAD.

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) 
	{
        std::cout << "Could not load OpenGL functions using GLAD." << std::endl;

        exit(4);
    }

    // Make sure the OpenGL version that was provided is greater than or equal
    // to OpenGL 3.3.

    if (GLVersion.major * 10 + GLVersion.minor < 33)
    {
    	std::cout << "Could not load OpenGL 3.3 using GLAD. Received OpenGL " << GLVersion.major << "." << GLVersion.minor << " instead." << std::endl;

        exit(5);
    }

    // Set up depth clamping.

    glEnable(GL_DEPTH_CLAMP);

    // Set up backface culling.

    glCullFace(GL_BACK);

	glFrontFace(GL_CW);

    // Load the block texture array.

    GLuint block_texture_array = load_block_texture_array();

    // Load the block face_info* array.

    load_block_face_info_array();

    // Load the block shader program.

    GLuint block_shader_program = load_program("../glsl/block_vertex.glsl", "../glsl/block_fragment.glsl");

    // Create an empty pointer to a world*.

    world* the_world = nullptr;

    // Define variables to hold the looking direction of the player.

    float rot_x_deg = 0.0f;
    float rot_y_deg = 0.0f;

    // Define variables to hold the position and velocity of the player.

    float player_x = 0.0f;
    float player_y = 0.0f;
    float player_z = 0.0f;

    float player_vx = 0.0f;
    float player_vy = 0.0f;
    float player_vz = 0.0f; 

    // Define the player's acceleration constant.

	const float acceleration = 0.025f;

	// Define the player's friction constant.

	const float friction = 0.9f;

    // When the block_timer is 0, a block may be destroyed or placed.

    int block_timer = 0;

    // If the save file exists, load the_world from the save file. Otherwise,
    // generate a new world and save it to the save file.

    if (std::ifstream(path_to_level).good())
    {
    	load_world_from_file
    	(
    		the_world, 

    		player_x,
    		player_y,
    		player_z,

    		path_to_level
    	);
    }
    else
    {
    	the_world = allocate_world(512, 128, 512);

    	generate_world(the_world, time(NULL));

    	player_x = float(the_world->x_res) / 2.0f;

    	player_y = 0.0f;
    	
    	player_z = float(the_world->z_res) / 2.0f;

    	save_world_to_file
    	(
    		the_world, 

    		player_x,
    		player_y,
    		player_z,

    		path_to_level
    	);
    }

    // Allocate a new accessor* from the_world.

    accessor* the_accessor = allocate_accessor(the_world);

    // Define the view distance.

    float view_distance = 256.0f;

    // Define the reach distance.

    float reach_distance = 8.0f;

    // Create variables to store the position of the mouse pointer, the state 
    // of the mouse buttons, and the relative mouse mode.

    int sdl_mouse_x = 0;
    int sdl_mouse_y = 0;

    bool sdl_mouse_l = false;
    bool sdl_mouse_r = false;

    bool sdl_mouse_relative = false;

    // The sdl_iteration counter is incremented every frame.

    unsigned long long sdl_iteration = 0;

    // Enter the main loop.

    bool sdl_running = true;

    while (sdl_running)
    {
    	// Remember the time at the start of the frame. At the end of the 
    	// frame, this timestamp will be used to cap the framerate.

    	auto frame_start_time = std::chrono::high_resolution_clock::now();

    	// Get the size of the sdl_window*, in case it was resized.

    	SDL_GetWindowSize(sdl_window, &sdl_x_res, &sdl_y_res);

    	// Poll events.

    	SDL_Event e;

    	while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				// The application was quit.

				sdl_running = false;
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				// The mouse moved.

				if (sdl_mouse_relative)
				{
					sdl_mouse_x += e.motion.xrel;
					sdl_mouse_y += e.motion.yrel;

					if (sdl_mouse_y > sdl_y_res - 1)
					{
						sdl_mouse_y = sdl_y_res - 1;
					}
					else if (sdl_mouse_y < 0)
					{
						sdl_mouse_y = 0;
					}
				}
				else
				{
					sdl_mouse_x = e.motion.x;
					sdl_mouse_y = e.motion.y;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				// A mouse button was pressed.

				if (e.button.button == SDL_BUTTON_LEFT)
				{
					sdl_mouse_l = true;
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					sdl_mouse_r = true;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				// A mouse button was released.

				if (e.button.button == SDL_BUTTON_LEFT)
				{
					sdl_mouse_l = false;
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					sdl_mouse_r = false;
				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				// A key was pressed.

				SDL_Keycode key = e.key.keysym.sym;

				if (key == SDLK_ESCAPE)
				{
					if (sdl_mouse_relative)
					{
						SDL_SetRelativeMouseMode(SDL_FALSE);

						sdl_mouse_relative = SDL_GetRelativeMouseMode();
					}
					else
					{
						sdl_running = false;
					}
				}
			}
		}

		// Enable relative mouse mode when the left mouse button is down and
		// relative mouse mode is currently off.

		if (sdl_mouse_relative == false && sdl_mouse_l == true)
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);

			sdl_mouse_relative = SDL_GetRelativeMouseMode();
		}

		// Calculate the looking direction of the camera.

		float rot_x_deg_want = (float(sdl_mouse_y) - (float(sdl_y_res) / 2.0f)) / float(sdl_y_res) * 180.0f;
		float rot_y_deg_want = (float(sdl_mouse_x) - (float(sdl_x_res) / 2.0f)) / float(sdl_x_res) * 360.0f;

		rot_x_deg += (rot_x_deg_want - rot_x_deg) / 16.0f;
		rot_y_deg += (rot_y_deg_want - rot_y_deg) / 16.0f;

		// Get the keyboard state.

		const Uint8* keys = SDL_GetKeyboardState(NULL);

		// Handle player movement.

		if (keys[SDL_SCANCODE_W])
		{
			player_vx -= sin(glm::radians(-rot_y_deg)) * acceleration;
			player_vz -= cos(glm::radians(-rot_y_deg)) * acceleration;

			player_vy -= sin(glm::radians(-rot_x_deg)) * acceleration;
		}
		else if (keys[SDL_SCANCODE_S])
		{
			player_vx += sin(glm::radians(-rot_y_deg)) * acceleration;
			player_vz += cos(glm::radians(-rot_y_deg)) * acceleration;

			player_vy += sin(glm::radians(-rot_x_deg)) * acceleration;
		}

		if (keys[SDL_SCANCODE_A])
		{
			player_vx -= sin(glm::radians(-rot_y_deg + 90.0f)) * acceleration;
			player_vz -= cos(glm::radians(-rot_y_deg + 90.0f)) * acceleration;
		}
		else if (keys[SDL_SCANCODE_D])
		{
			player_vx += sin(glm::radians(-rot_y_deg + 90.0f)) * acceleration;
			player_vz += cos(glm::radians(-rot_y_deg + 90.0f)) * acceleration;
		}

		if (keys[SDL_SCANCODE_SPACE])
		{
			player_vy -= acceleration;
		}
		else if (keys[SDL_SCANCODE_LSHIFT])
		{
			player_vy += acceleration;
		}

		// Multiply the player's velocity by the player's friction constant.

		player_vx *= friction;
		player_vy *= friction;
		player_vz *= friction;

		// Create a list of hitboxes of nearby blocks.

		std::vector<hitbox> near_hitboxes;

		int b_res = 3;

		for (int x = -b_res; x <= b_res; x++)
		for (int y = -b_res; y <= b_res; y++)
		for (int z = -b_res; z <= b_res; z++)
		{
			block_id near_id = the_world->get_id_safe(player_x + x, player_y + y, player_z + z);

			if (near_id != id_air && near_id != id_null)
			{
				near_hitboxes.push_back
				(
					hitbox
					(
						floor(player_x + x),
						floor(player_y + y),
						floor(player_z + z),

						1.0f,
						1.0f,
						1.0f
					)
				);
			}
		}

		// Define the player's hitbox.

		hitbox player_hitbox = hitbox(player_x, player_y, player_z, 0.6f, 1.8f, 0.6f);

		// Do collision detection and response.

		player_hitbox.x += player_vx;

		for (int i = 0; i < near_hitboxes.size(); i++)
		{
			hitbox near_hitbox = near_hitboxes[i];

			if (hitbox_intersect(player_hitbox, near_hitbox))
			{
				player_hitbox.x += hitbox_x_depth(player_hitbox, near_hitbox);

				player_vx = 0.0f;
			}
		}

		player_hitbox.y += player_vy;

		for (int i = 0; i < near_hitboxes.size(); i++)
		{
			hitbox near_hitbox = near_hitboxes[i];

			if (hitbox_intersect(player_hitbox, near_hitbox))
			{
				player_hitbox.y += hitbox_y_depth(player_hitbox, near_hitbox);

				player_vy = 0.0f;
			}
		}

		player_hitbox.z += player_vz;

		for (int i = 0; i < near_hitboxes.size(); i++)
		{
			hitbox near_hitbox = near_hitboxes[i];

			if (hitbox_intersect(player_hitbox, near_hitbox))
			{
				player_hitbox.z += hitbox_z_depth(player_hitbox, near_hitbox);

				player_vz = 0.0f;
			}
		}

		player_x = player_hitbox.x;
		player_y = player_hitbox.y;
		player_z = player_hitbox.z;

		// Block placement and deletion testing.

		if ((sdl_mouse_l || sdl_mouse_r) && block_timer == 0)
		{
			float px = player_x;
			float py = player_y;
			float pz = player_z;

			float ix = -sin(glm::radians(-rot_y_deg));
			float iy = -tan(glm::radians(-rot_x_deg));
			float iz = -cos(glm::radians(-rot_y_deg));

			float total_distance = 0.0f;

			while (true)
			{
				// Check for collisions.

				block_id collision = the_world->get_id_safe(px, py, pz);

				if (collision != id_air && collision != id_null)
				{
					if (sdl_mouse_l)
					{
						px -= ix * 0.001f;
						py -= iy * 0.001f;
						pz -= iz * 0.001f;

						the_accessor->set_id_safe(px, py, pz, id_cobblestone);
					}
					else
					{
						the_accessor->set_id_safe(px, py, pz, id_air);
					}

					block_timer = 10;

					break;
				}

				px += ix * 0.001f;
				py += iy * 0.001f;
				pz += iz * 0.001f;

				total_distance += 0.001f;

				if (total_distance > reach_distance)
				{
					break;
				}
			}
		}

		// Update all modified chunks.

		for (int i = 0; i < the_accessor->chunk_count; i++)
		{
			chunk*& the_chunk = the_accessor->the_chunks[i];

			if (the_chunk->modified)
			{
				chunk* new_chunk = allocate_chunk
				(
					the_world,

					the_chunk->x,
					the_chunk->y,
					the_chunk->z,

					the_chunk->x_res,
					the_chunk->y_res,
					the_chunk->z_res
				);

				deallocate_chunk(the_chunk);

				the_chunk = new_chunk;
			}
		}

		// Clear the OpenGL context to the default Minceraft sky color.

		glClearColor(186.0f / 255.0f, 214.0f / 255.0f, 254.0f / 255.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render the world.

		{
			// Enable depth testing.

			glEnable(GL_DEPTH_TEST);

			// Enable backface culling.

			glEnable(GL_CULL_FACE);

			// Bind the block shader program to the current state.

			glUseProgram(block_shader_program);

			// Calculate the projection, view, and model matrices, and pass 
			// them to the block_shader_program.

			{
				// Calculate the aspect ratio.

				float aspect_ratio = (float)sdl_x_res / (float)sdl_y_res;

				// Generate the projection matrix.

				glm::mat4 matrix_projection = glm::perspective(glm::radians(70.0f), aspect_ratio, 0.128f, 1024.0f);

				// Generate the view matrix.

				glm::mat4 matrix_view = glm::mat4(1.0f);

				matrix_view = glm::rotate(matrix_view, glm::radians(rot_x_deg), glm::vec3(1.0f, 0.0f, 0.0f));
				matrix_view = glm::rotate(matrix_view, glm::radians(rot_y_deg), glm::vec3(0.0f, 1.0f, 0.0f));

				// Generate the model matrix. The translation component must 
				// be centered around the X and Z axes of the player hitbox,
				// and must be a small amount lower than the top of the player
				// hitbox.

				glm::mat4 matrix_model = glm::translate(glm::mat4(1.0f), glm::vec3(-player_x - player_hitbox.xr / 2.0f, player_y + 0.2f, -player_z - player_hitbox.zr / 2.0f));

				// Pass the matrices to the block_shader_program.

				glUniformMatrix4fv(glGetUniformLocation(block_shader_program, "matrix_projection"), 1, GL_FALSE, &matrix_projection[0][0]);

				glUniformMatrix4fv(glGetUniformLocation(block_shader_program, "matrix_view"), 1, GL_FALSE, &matrix_view[0][0]);

				glUniformMatrix4fv(glGetUniformLocation(block_shader_program, "matrix_model"), 1, GL_FALSE, &matrix_model[0][0]);
			}

			// Pass the fog distance to the block_shader_program.

			glUniform1f(glGetUniformLocation(block_shader_program, "fog_distance"), view_distance * view_distance);

			// Pass the current time (in seconds) to the block_shader_program.

			glUniform1f(glGetUniformLocation(block_shader_program, "time_in_seconds"), SDL_GetTicks() / 1000.0f);

			// Bind the block_texture_array to the current state.

			glBindTexture(GL_TEXTURE_2D_ARRAY, block_texture_array);

			// Render all of the chunks' vertex arrays in the_accessor.

			for (int i = 0; i < the_accessor->chunk_count; i++)
			{
				chunk* the_chunk = the_accessor->the_chunks[i];

				float ccx = the_chunk->x + (the_chunk->x_res / 2);
				float ccy = the_chunk->y + (the_chunk->y_res / 2);
				float ccz = the_chunk->z + (the_chunk->z_res / 2);

				float dx = ccx - player_x;
				float dy = ccy - player_y;
				float dz = ccz - player_z;

				if (dx * dx + dy * dy + dz * dz < view_distance * view_distance)
				{
					render_chunk(the_chunk);
				}
			}

			// Disable writing to the depth buffer.

			glDepthMask(GL_FALSE);

			// Enable alpha blending.

			glEnable(GL_BLEND);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Render all of the chunks' water vertex arrays in the_accessor.

			for (int i = 0; i < the_accessor->chunk_count; i++)
			{
				chunk* the_chunk = the_accessor->the_chunks[i];

				float ccx = the_chunk->x + (the_chunk->x_res / 2);
				float ccy = the_chunk->y + (the_chunk->y_res / 2);
				float ccz = the_chunk->z + (the_chunk->z_res / 2);

				float dx = ccx - player_x;
				float dy = ccy - player_y;
				float dz = ccz - player_z;

				if (dx * dx + dy * dy + dz * dz < view_distance * view_distance)
				{
					render_chunk_water(the_chunk);
				}
			}

			// Enable writing to the depth buffer.

			glDepthMask(GL_TRUE);

			// Disable alpha blending.

			glDisable(GL_BLEND);

			// Unbind the block_texture_array from the current state.

			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

			// Unbind the block shader program from the current state.

			glUseProgram(0);

			// Disable backface culling.

			glDisable(GL_CULL_FACE);

			// Disable depth testing.

			glDisable(GL_DEPTH_TEST);
		}

		// Swap the back buffer to the front.

		SDL_GL_SwapWindow(sdl_window);

		// Cap the framerate to 60 Hz.

		float frame_elapsed_time = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - frame_start_time).count();

		if (frame_elapsed_time < 1000.0f / 60.0f)
		{
			int frame_sleep_time = round(1000.0f / 60.0f - frame_elapsed_time);

			std::this_thread::sleep_for(std::chrono::milliseconds(frame_sleep_time));
		}

		// Decrement the block timer, if the block timer is not equal to 0.

		if (block_timer != 0)
		{
			block_timer--;
		}

		// Increment the iteration counter. Print the framerate every 60 
		// iterations.

		sdl_iteration++;

		if (sdl_iteration % 60 == 0)
		{
			std::cout << "Running at " << 1000.0f / frame_elapsed_time << " Hz" << std::endl;
		}
    }

    // Save the world to the save file.

    save_world_to_file
	(
		the_world, 

		player_x,
		player_y,
		player_z,

		path_to_level
	);

    // Destroy all Minceraft related objects.

    deallocate_world(the_world);

    deallocate_accessor(the_accessor);

    // Destroy all OpenGL related objects.

    glDeleteTextures(1, &block_texture_array);

    glDeleteProgram(block_shader_program);

    SDL_GL_DeleteContext(gl_context);

    // Destroy all SDL related objects.

    SDL_DestroyWindow(sdl_window);

    SDL_Quit();

    // Exit cleanly.

    exit(EXIT_SUCCESS);
}