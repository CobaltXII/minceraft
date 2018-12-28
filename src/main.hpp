#include <iostream>
#include <chrono>
#include <thread>

// GLM is used as the mathematics library. It provides functions that are 
// based on GLSL, and makes it really easy to generate projection and view
// matrices.

#include <glm/vec3.hpp>

#include <glm/mat4x4.hpp>

#include <glm/gtc/matrix_transform.hpp>

// GLAD is used as the loader for OpenGL functions. On operating systems such
// as OS X, a loader is not necessarily needed. However, on most other 
// systems, a loader is required in order to use any OpenGL functions.

#include <glad/glad.h>

// SDL2 is used as the multimedia interface for Minceraft. It provides
// cross-platform functions that allow easy access to the operating system.

#include <SDL.h>

#include <shader.hpp>
#include <shader.cpp>

#include <program.hpp>
#include <program.cpp>