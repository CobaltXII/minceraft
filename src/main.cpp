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

	// Request a 24-bit depth buffer. 24-bit depth buffers are supported on
	// most modern hardware. Having a floating-point precision of lower than
	// 24-bits will cause graphical issues.

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Enable vertical retrace synchronization. If this is not enabled, a
	// phenomenon known as tearing occurs, which causes half of the screen to
	// retain the contents of the last frame instead of the entire screen 
	// updating immediately.

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

    // Set up preliminaries for backface culling. Backface culling is a way to
    // speed up rendering by culling triangles that face away from the viewer.

    glCullFace(GL_BACK);

	glFrontFace(GL_CW);

    // Load the block texture array.

    GLuint block_texture_array = load_block_texture_array();

    // Load the block shader program.

    GLuint block_shader_program = load_program("../glsl/block_vertex.glsl", "../glsl/block_fragment.glsl");

    // Create variables to represent the position of the mouse pointer, and
    // state of the mouse buttons.

    int sdl_mouse_x = 0;
    int sdl_mouse_y = 0;

    bool sdl_mouse_l = false;
    bool sdl_mouse_r = false;

    // The sdl_iteration counter is incremented every frame. It should never
    // be used for timing purposes, use SDL_GetTicks for that.

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

		// Clear the OpenGL context to the default Minceraft sky color. This
		// might be changed to use a 'time-of-day' variable instead.

		glClearColor(186.0f / 255.0f, 214.0f / 255.0f, 254.0f / 255.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			// Allocate a block of memory to hold the vertices of a cube.

			float* cube_vertices = (float*)malloc(6 * 2 * 3 * 7 * sizeof(float));

			// Which faces should be generated?

			bool visible_top = true;

			bool visible_bottom = true;

			bool visible_left = true;

			bool visible_right = true;

			bool visible_front = true;

			bool visible_back = true;

			// What is the layer number of each face?

			float layer_top = block_name_to_layer.at("grass_top");

			float layer_bottom = block_name_to_layer.at("dirt");

			float layer_left = block_name_to_layer.at("grass_side");

			float layer_right = block_name_to_layer.at("grass_side");

			float layer_front = block_name_to_layer.at("grass_side");

			float layer_back = block_name_to_layer.at("grass_side");

			// What is the lighting value of each face?

			float lighting_top = 1.0f;

			float lighting_bottom = 1.0f;

			float lighting_left = 0.75f;

			float lighting_right = 0.75f;

			float lighting_front = 0.9f;

			float lighting_back = 0.9f;

			// What is the offset of the cube?

			float ox = -0.5f;
			float oy = -0.5f;
			float oz = -0.5f;

			// It is really simple and efficient to use pointer arithmetic to
			// write to blocks of memory. So, that's what this function will 
			// use.

			float* ptr = cube_vertices;

			// Generate the faces.

			if (visible_top)
			{
				// Top face.

				*(ptr++) = 1.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
				*(ptr++) = 0.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
				*(ptr++) = 0.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;

				*(ptr++) = 1.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
				*(ptr++) = 0.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
				*(ptr++) = 1.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_top; *(ptr++) = lighting_top;
			}

			if (visible_bottom)
			{
				// Bottom face.

				*(ptr++) = 0.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
				*(ptr++) = 0.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
				*(ptr++) = 1.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;

				*(ptr++) = 0.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
				*(ptr++) = 1.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
				*(ptr++) = 1.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_bottom; *(ptr++) = lighting_bottom;
			}

			if (visible_left)
			{
				// Left face.

				*(ptr++) = 0.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
				*(ptr++) = 0.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
				*(ptr++) = 0.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;

				*(ptr++) = 0.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
				*(ptr++) = 0.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
				*(ptr++) = 0.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_left; *(ptr++) = lighting_left;
			}

			if (visible_right)
			{
				// Right face.

				*(ptr++) = 1.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
				*(ptr++) = 1.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
				*(ptr++) = 1.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;

				*(ptr++) = 1.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
				*(ptr++) = 1.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
				*(ptr++) = 1.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_right; *(ptr++) = lighting_right;
			}

			if (visible_front)
			{
				// Front face.

				*(ptr++) = 1.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
				*(ptr++) = 1.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
				*(ptr++) = 0.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;

				*(ptr++) = 1.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
				*(ptr++) = 0.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
				*(ptr++) = 0.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 1.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_front; *(ptr++) = lighting_front;
			}

			if (visible_back)
			{
				// Back face.

				*(ptr++) = 0.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
				*(ptr++) = 0.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 1.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
				*(ptr++) = 1.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;

				*(ptr++) = 0.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 1.0f; *(ptr++) = 0.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
				*(ptr++) = 1.0f + ox; *(ptr++) = -1.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 1.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
				*(ptr++) = 1.0f + ox; *(ptr++) = -0.0f - oy; *(ptr++) = 0.0f + oz; *(ptr++) = 0.0f; *(ptr++) = 0.0f; *(ptr++) = layer_back; *(ptr++) = lighting_back;
			}

			// How much of the allocated memory was used (in bytes)?

			unsigned int cube_vertices_size = (ptr - cube_vertices) * sizeof(float);

			// Generate a VAO and a VBO to reference the vertex data of the
			// cube after it is uploaded to the GPU.

			GLuint cube_vao;
			GLuint cube_vbo;

			glGenVertexArrays(1, &cube_vao);

			glGenBuffers(1, &cube_vbo);

			// Bind the VAO and the VBO to the current state.

			glBindVertexArray(cube_vao);

			glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);

			// Upload the vertex data to the GPU.

			glBufferData(GL_ARRAY_BUFFER, cube_vertices_size, cube_vertices, GL_STATIC_DRAW);

			// Enable the appropriate vertex attributes.

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0 * sizeof(float)));

			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));

			glEnableVertexAttribArray(2);

			{
				// Enable depth testing.

				glEnable(GL_DEPTH_TEST);

				// Enable backface culling.

				glEnable(GL_CULL_FACE);

				// Bind the block shader program to the current state.

				glUseProgram(block_shader_program);

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

					glm::mat4 matrix_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

					matrix_model = glm::rotate(matrix_model, glm::radians(sin(SDL_GetTicks() / 1000.0f) * 90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					matrix_model = glm::rotate(matrix_model, glm::radians(cos(SDL_GetTicks() / 1000.0f) * 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

					// Pass the matrices to the block shader program.

					glUniformMatrix4fv(glGetUniformLocation(block_shader_program, "matrix_projection"), 1, GL_FALSE, &matrix_projection[0][0]);

					glUniformMatrix4fv(glGetUniformLocation(block_shader_program, "matrix_view"), 1, GL_FALSE, &matrix_view[0][0]);

					glUniformMatrix4fv(glGetUniformLocation(block_shader_program, "matrix_model"), 1, GL_FALSE, &matrix_model[0][0]);
				}

				// Bind the block_texture_array to the current state.

				glBindTexture(GL_TEXTURE_2D_ARRAY, block_texture_array);

				// Draw the cube_vertices as an array of triangles.

				glDrawArrays(GL_TRIANGLES, 0, cube_vertices_size / sizeof(float) / 7);

				// Unbind the block_texture_array from the current state.

				glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

				// Unbind the block shader program from the current state.

				glUseProgram(0);

				// Disable backface culling.

				glDisable(GL_CULL_FACE);

				// Disable depth testing.

				glDisable(GL_DEPTH_TEST);
			}

			// Unbind the VAO and the VBO from the current state.

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);

			// Destroy the VAO and the VBO, since they are no longer going to
			// be used.

			glDeleteBuffers(1, &cube_vbo);

			glDeleteVertexArrays(1, &cube_vao);

			// Free the block of memory allocated to hold the vertices of a
			// cube.

			free(cube_vertices);
		}

		// Swap the back buffer to the front, so that the last frame will 
		// appear on the screen.

		SDL_GL_SwapWindow(sdl_window);

		// Cap the framerate to 60 Hz.

		float frame_elapsed_time = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - frame_start_time).count();

		if (frame_elapsed_time < 1000.0f / 60.0f)
		{
			int frame_sleep_time = round(1000.0f / 60.0f - frame_elapsed_time);

			std::this_thread::sleep_for(std::chrono::milliseconds(frame_sleep_time));
		}

		// Increment the iteration counter. Do not use the iteration counter
		// as a precise time measurement!

		sdl_iteration++;

		if (sdl_iteration % 60 == 0)
		{
			std::cout << "Running at " << 1000.0f / frame_elapsed_time << " Hz" << std::endl;
		}
    }

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