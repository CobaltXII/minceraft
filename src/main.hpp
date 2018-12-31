#include <iostream>
#include <chrono>
#include <thread>

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
#include <shader.cpp>

#include <program.hpp>
#include <program.cpp>

#include <image.hpp>
#include <image.cpp>

#include <texture.hpp>
#include <texture.cpp>

#include <block.hpp>
#include <block.cpp>

#include <voxel.hpp>
#include <voxel.cpp>

#include <face.hpp>
#include <face.cpp>

#include <world.hpp>
#include <world.cpp>

#include <mesh.hpp>
#include <mesh.cpp>

#include <chunk.hpp>
#include <chunk.cpp>

#include <skylight.hpp>
#include <skylight.cpp>

#include <accessor.hpp>
#include <accessor.cpp>

#include <generator.hpp>
#include <generator.cpp>

#include <file.hpp>
#include <file.cpp>