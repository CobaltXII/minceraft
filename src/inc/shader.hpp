#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Load a file as a string. The string must have two or more null-terminator
// characters as a suffix, otherwise OpenGL will not parse the contents
// correctly.

std::string load_file(std::string path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cout << "Could not open file \"" << path << "\"." << std::endl;

		exit(6);
	}

	std::stringstream buffer;

	buffer << file.rdbuf() << "\0";

	return buffer.str();
}

// Load a shader from a file.

GLuint load_shader(std::string path, GLenum shader_type)
{
	GLuint shader = glCreateShader(shader_type);

	std::string file_contents = load_file(path);

	const GLchar* file_data = file_contents.c_str();

	glShaderSource(shader, 1, &file_data, NULL);

	glCompileShader(shader);

	GLint success = 0;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		std::cout << "Could not compile shader loaded from \"" << path << "\"." << std::endl;

		GLchar crash_information[4096];

		glGetShaderInfoLog(shader, 4096, NULL, crash_information);

		std::cout << crash_information;

		exit(7);
	}

	return shader;
}