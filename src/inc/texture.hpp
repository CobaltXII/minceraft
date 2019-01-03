#include <iostream>
#include <vector>
#include <string>
#include <map>

// Generates a std::map that maps block texture names to layer indices in a 2D
// texture array. Returns the OpenGL identifier of that 2D texture array.

std::map<std::string, float> block_name_to_layer;

GLuint load_block_texture_array()
{
	// Define the expected properties of each sub-texture.
	
	int x_res = 16;
	int y_res = 16;

	int channels = 4;

	// The texture array must have one layer for each texture path in all_tex.

	GLsizei layers = all_tex.size();

	// Allocate space for the texels.

	GLubyte* texels = (GLubyte*)malloc(x_res * y_res * channels * layers * sizeof(GLubyte));

	if (!texels)
	{
		std::cout << "Could not allocate enough memory for a texture array." << std::endl;

		exit(9);
	}

	// Load the textures.

	for (int i = 0; i < layers; i++)
	{
		// Load the sub-texture.

		std::string path = "../tex/" + all_tex[i] + ".png";

		int sub_x_res;
		int sub_y_res;

		int sub_channels;

		unsigned char* sub_data = stbi_load(path.c_str(), &sub_x_res, &sub_y_res, &sub_channels, STBI_rgb_alpha);

		if (!sub_data)
		{
			std::cout << "Could not load \"" << path << "\" using stbi_load." << std::endl;

			exit(10);
		}

		if (sub_x_res != x_res || sub_y_res != y_res)
		{
			std::cout << "Dimensions of \"" << path << "\" (" << sub_x_res << ", " << sub_y_res << ") do not match the expected dimensions (" << x_res << ", " << y_res << ")." << std::endl;
		
			exit(11);
		}

		if (sub_channels != channels)
		{
			std::cout << "Channel count of \"" << path << "\" (" << sub_channels << ") does not match the expected channel count (" << channels << ")." << std::endl;

			exit(12); 
		}

		// Copy the sub-texture's texels to the texel array.

		memcpy(texels + (i * x_res * y_res * channels), sub_data, x_res * y_res * channels);

		// Free the sub-texture.

		stbi_image_free(sub_data);

		// Add a map entry that has a key of the sub-texture's name and a 
		// value of the sub-texture's layer index to the map.

		block_name_to_layer.emplace(all_tex[i], float(i));
	}

	// Generate the texture array.

	GLuint texture_array = 0;

	glGenTextures(1, &texture_array);

	// Bind the texture array to the current state.

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);

	// Allocate storage for the 2D texture array.

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, x_res, y_res, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Upload the texel data.

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, x_res, y_res, layers, GL_RGBA, GL_UNSIGNED_BYTE, texels);

	// Set up texture parameters.

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Unbind the texture array from the current state.

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	// Free the texels.

	free(texels);

	// Return the reference to the texture array.

	return texture_array;
}

// A texture that has been loaded on to the CPU and the GPU.

struct complete_texture
{
	// The reference to the OpenGL texture.

	GLuint storage;

	// The dimensions of the texture.

	int x_res;
	int y_res;

	// The channel count of the texture.

	int channels;

	// The pixels of the texture. 

	unsigned char* pixels;
};

// Load the interface textures into a std::map mapping the interface texture's
// name to it's complete_texture*.

std::map<std::string, complete_texture*> interface_textures;

void load_interface_textures()
{
	for (int i = 0; i < all_gui.size(); i++)
	{
		// Load the texture.

		std::string path = "../gui/" + all_gui[i] + ".png";

		GLuint gui_texture;

		glGenTextures(1, &gui_texture);

		glBindTexture(GL_TEXTURE_2D, gui_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int gui_x_res;
		int gui_y_res;

		int gui_channels;

		unsigned char* gui_data = stbi_load(path.c_str(), &gui_x_res, &gui_y_res, &gui_channels, 0);

		if (!gui_data)
		{
			std::cout << "Could not load \"" << path << "\" using stbi_load." << std::endl;

			exit(18);
		}

		if (gui_channels != 4 && gui_channels != 3)
		{
			std::cout << "Channel count of \"" << path << "\" (" << gui_channels << ") does not match any of the expected channel counts (3 or 4)." << std::endl;

			exit(19); 
		}

		if (gui_channels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gui_x_res, gui_y_res, 0, GL_RGBA, GL_UNSIGNED_BYTE, gui_data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, gui_x_res, gui_y_res, 0, GL_RGB, GL_UNSIGNED_BYTE, gui_data);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		// Generate a complete_texture* to store in interface_textures.

		complete_texture* gui_complete = new complete_texture();

		gui_complete->storage = gui_texture;

		gui_complete->x_res = gui_x_res;
		gui_complete->y_res = gui_y_res;

		gui_complete->channels = gui_channels;

		gui_complete->pixels = gui_data;

		// Add the complete_texture entry to interface_textures.

		interface_textures.emplace(all_gui[i], gui_complete);
	}
}