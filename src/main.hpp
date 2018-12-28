#include <iostream>
#include <chrono>
#include <thread>

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