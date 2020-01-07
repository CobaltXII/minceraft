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

// A GUI texture.
struct gui_texture {
	GLuint id;
	float w;
	float h;
};

// Load a texture from an image.
gui_texture load_gui(std::string path0) {
	std::string path = "../gui/" + path0 + ".png";
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width;
	int height;
	int channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (!data) {
		std::cout << "Could not load \"" << path << "\" using stbi_load." << std::endl;
		exit(EXIT_FAILURE);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return {textureID, float(width), float(height)};
}