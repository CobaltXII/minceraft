/*

Minceraft

By CobaltXII

*/

#include "main.hpp"

// The entry point.

int main(int argc, char** argv)
{
	// Initialize SDL.

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		std::cout << "Could not initialize SDL." << std::endl;

		exit(1);
	}

	// The dimensions of the SDL_Window*.

	int sdl_x_res = 840;
	int sdl_y_res = 525;

	// Create the SDL_Window*.

	SDL_Window* sdl_window = SDL_CreateWindow
	(
		"Minceraft 0.0.0",

		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,

		sdl_x_res,
		sdl_y_res,

		SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL
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

    // Allocate a new world*.

    world* the_world = allocate_world(32, 32, 32);

    for (int x = 0; x < the_world->x_res; x++)
    for (int y = 0; y < the_world->y_res; y++)
    for (int z = 0; z < the_world->z_res; z++)
    {
    	if (rand() % 5 == 0)
    	{
    		the_world->set_id(x, y, z, block_id(rand() % id_null));
    	}
    	else
    	{
    		the_world->set_id(x, y, z, id_air);
    	}

    	the_world->set_natural(x, y, z, 15);
    }

    // Allocate a new accessor* from the_world.

    accessor* the_accessor = allocate_accessor(the_world);

    // Define variables to hold the looking direction of the player.

    float rot_x_deg = 0.0f;
    float rot_y_deg = 0.0f;

    // Define variables to hold the position and velocity of the player.

    float player_x = float(the_world->x_res) / 2.0f;
    float player_y = float(the_world->y_res) / 2.0f;
    float player_z = float(the_world->z_res) / 2.0f;

    float player_vx = 0.0f;
    float player_vy = 0.0f;
    float player_vz = 0.0f; 

	// Define the player's acceleration constant.

	const float acceleration = 0.025f;

	// Define the player's friction constant.

	const float friction = 0.9f;

    // Create variables to store the position of the mouse pointer and the 
    // state of the mouse buttons.

    int sdl_mouse_x = 0;
    int sdl_mouse_y = 0;

    bool sdl_mouse_l = false;
    bool sdl_mouse_r = false;

    // The sdl_iteration counter is incremented every frame.

    unsigned long long sdl_iteration = 0;

    // Enter the main loop.

    bool sdl_running = true;

    while (sdl_running)
    {
    	// Remember the time at the start of the frame. At the end of the 
    	// frame, this timestamp will be used to cap the framerate.

    	auto frame_start_time = std::chrono::high_resolution_clock::now();

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

				sdl_mouse_x = e.motion.x;
				sdl_mouse_y = e.motion.y;
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
					sdl_running = false;
				}
			}
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

		// Update the player's velocity and position.

		player_vx *= friction;
		player_vy *= friction;
		player_vz *= friction;

		player_x += player_vx;
		player_y += player_vy;
		player_z += player_vz;

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

				glm::mat4 matrix_projection = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.128f, 256.0f);

				// Calculate the looking direction of the camera.

				float rot_x_deg = (float(sdl_mouse_y) - (float(sdl_y_res) / 2.0f)) / float(sdl_y_res) * 180.0f;
				float rot_y_deg = (float(sdl_mouse_x) - (float(sdl_x_res) / 2.0f)) / float(sdl_x_res) * 360.0f;

				// Generate the view matrix.

				glm::mat4 matrix_view = glm::mat4(1.0f);

				matrix_view = glm::rotate(matrix_view, glm::radians(rot_x_deg), glm::vec3(1.0f, 0.0f, 0.0f));
				matrix_view = glm::rotate(matrix_view, glm::radians(rot_y_deg), glm::vec3(0.0f, 1.0f, 0.0f));

				// Generate the model matrix.

				glm::mat4 matrix_model = glm::translate(glm::mat4(1.0f), glm::vec3(-16.0f, +16.0f, -48.0f));

				// Pass the matrices to the block shader program.

				glUniformMatrix4fv(glGetUniformLocation(block_shader_program, "matrix_projection"), 1, GL_FALSE, &matrix_projection[0][0]);

				glUniformMatrix4fv(glGetUniformLocation(block_shader_program, "matrix_view"), 1, GL_FALSE, &matrix_view[0][0]);

				glUniformMatrix4fv(glGetUniformLocation(block_shader_program, "matrix_model"), 1, GL_FALSE, &matrix_model[0][0]);
			}

			// Bind the block_texture_array to the current state.

			glBindTexture(GL_TEXTURE_2D_ARRAY, block_texture_array);

			// Render all of the chunks in the_accessor.

			for (int i = 0; i < the_accessor->chunk_count; i++)
			{
				render_chunk(the_accessor->the_chunks[i]);
			}

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

		// Increment the iteration counter. Print the framerate every 60 
		// iterations.

		sdl_iteration++;

		if (sdl_iteration % 60 == 0)
		{
			std::cout << "Running at " << 1000.0f / frame_elapsed_time << " Hz" << std::endl;
		}
    }

    // Destroy all Minceraft related objects.

    deallocate_world(the_world);

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