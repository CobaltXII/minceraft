#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>

// GLM is used as the mathematics library.

#include <glm/vec3.hpp>

#include <glm/mat4x4.hpp>

#include <glm/gtc/matrix_transform.hpp>

// GLAD is used as the loader for OpenGL functions.

#include <glad/glad.h>

// SDL2 is used as the multimedia interface.

#include <SDL.h>

// stb_image is used as the image loading library.

#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

// FastNoise is used as the noise library.

#include <FastNoise/FastNoise.h>

// Local headers.

#include <shader.hpp>

#include <program.hpp>

#include <image.hpp>

#include <texture.hpp>

#include <block.hpp>

#include <voxel.hpp>

#include <face.hpp>

#include <world.hpp>

#include <mesh.hpp>

#include <chunk.hpp>

#include <skylight.hpp>

#include <accessor.hpp>

#include <generator.hpp>

#include <file.hpp>

#include <hitbox.hpp>

#include <sprite.hpp>

#include <button.hpp>